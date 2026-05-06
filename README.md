# Flashfire Cobra V6 - Hatswitch Kernel Fix

Este módulo de kernel é um driver customizado para o joystick **Flashfire Cobra V6**. Ele resolve um problema crônico onde o firmware do dispositivo reporta o hatswitch (POV/D-Pad) de forma incorreta (apenas um eixo), tornando-o inutilizável na maioria dos simuladores no Linux.

## 📋 O Problema
O Flashfire Cobra V6, em seu modo padrão, envia os dados do Hatswitch em um formato que o subsistema HID do Linux não interpreta como um seletor de direções padrão. Isso resulta em:
- Reconhecimento de apenas um eixo (X ou Y).
- Direções diagonais inexistentes.
- Conflitos de mapeamento em jogos como Elite Dangerous, DCS e MSFS.

## ✨ A Solução
Este driver intercepta os pacotes brutos (raw events) do hardware e remapeia o valor do Hatswitch para os eixos `ABS_HAT0X` e `ABS_HAT0Y` corretamente, habilitando as 8 direções padrão.

## 🛠️ Instalação Rápida

### Pré-requisitos
Certifique-se de ter os headers do kernel instalados (Exemplo em sistemas baseados em Debian/Ubuntu):
```bash
sudo apt install build-essential linux-headers-\$(uname -r)
```

### Compilação e Carga
1. Clone este repositório:
   ```bash
   git clone https://github.com
   cd cobra-v6-fix
   ```

2. Compile o módulo:
   ```bash
   make
   ```

3. Instale o módulo no kernel:
   ```bash
   sudo insmod cobra_v6.ko
   ```

## 🔄 Instalação via DKMS (Recomendado)
Para que o driver continue funcionando após atualizações de sistema:

```bash
sudo cp -R . /usr/src/cobra_v6-1.0
sudo dkms add -m cobra_v6 -v 1.0
sudo dkms build -m cobra_v6 -v 1.0
sudo dkms install -m cobra_v6 -v 1.0
```

## 🔍 Verificação
Após carregar o módulo, você pode testar o funcionamento com:
- `jstest /dev/input/js0`
- `evtest` (selecionando o Cobra V6)

## ⚖️ Licença
Distribuído sob a licença GPL v2.

