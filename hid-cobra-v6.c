#include <linux/hid.h>
#include <linux/module.h>

static int cobra_event(struct hid_device *hdev, struct hid_field *field,
			struct hid_usage *usage, __s32 value)
{
	/* Verifica se o evento vindo do hardware é o do Hat Switch */
	if (usage->hid == HID_GD_HATSWITCH) {
		int x = 0, y = 0;

		/* A nossa tabela de tradução "Ângulo -> Coordenada" */
		switch (value) {
			case 0:   x = 0;  y = -1; break; // Cima
			case 30:  x = 1;  y = -1; break; // Cima-Dir
			case 60:  x = 1;  y = 0;  break; // Dir
			case 90:  x = 1;  y = 1;  break; // Baixo-Dir
			case 120: x = 0;  y = 1;  break; // Baixo
			case 150: x = -1; y = 1;  break; // Baixo-Esq
			case 180: x = -1; y = 0;  break; // Esq
			case 210: x = -1; y = -1; break; // Cima-Esq
			default:  x = 0;  y = 0;  break; // Centro/Solto
		}

		/* Reporta os eixos X e Y separadamente para o sistema */
		input_report_abs(field->hidinput->input, ABS_HAT0X, x);
		input_report_abs(field->hidinput->input, ABS_HAT0Y, y);
		input_sync(field->hidinput->input);
		
		return 1; /* Retornar 1 diz ao Kernel: "Eu já resolvi isso, ignore o original" */
	}
	return 0;
}

static const struct hid_device_id cobra_devices[] = {
	{ HID_USB_DEVICE(0x11c0, 0x5606) }, /* Seu Betop/Cobra V6 */
	{ }
};
MODULE_DEVICE_TABLE(hid, cobra_devices);


static int cobra_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
    int ret;
    struct hid_input *hidinput;

    /* 1. Inicia o hardware */
    ret = hid_parse(hdev);
    if (ret) return ret;

    ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
    if (ret) return ret;

    /* 2. Força o suporte ao Eixo Y no dispositivo de entrada */
    /* Em vez de um loop complexo, pegamos a lista de inputs do hdev */
    list_for_each_entry(hidinput, &hdev->inputs, list) {
        struct input_dev *input = hidinput->input;
        
        /* Habilita o bit de evento absoluto e o eixo HAT0Y */
        set_bit(EV_ABS, input->evbit);
        input_set_abs_params(input, ABS_HAT0Y, -1, 1, 0, 0);
    }

    return 0;
}


static struct hid_driver cobra_driver = {
	.name = "hid-cobra-v6",
	.id_table = cobra_devices,
	.event = cobra_event,
    .probe = cobra_probe
};

module_hid_driver(cobra_driver);
MODULE_LICENSE("GPL");
