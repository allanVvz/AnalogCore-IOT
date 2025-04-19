# Projeto Controle Analógico com Arduino Pro Micro e Bluetooth

Este projeto implementa um **controle de videogame analógico** utilizando um **Arduino Pro Micro (ATmega32U4)**, joystick analógico, botões digitais e comunicação Bluetooth (MH‑10), com suporte a emulação HID (mouse/teclado) e transmissão de dados em JSON.

---

## 🔧 Hardware

### 1. Arduino Pro Micro (ATmega32U4)
- **Versões disponíveis:** 5V/16 MHz ou 3.3V/8 MHz
- **Tensão de operação:**
  - RAW: 3.6 V–12 V via pino RAW (regulador onboard)
  - VCC: 5 V (ou 3.3 V na versão correspondente) via pino VCC
- **Memória:** Flash 32 kB, SRAM 2.5 kB, EEPROM 1 kB
- **Periféricos:** USB nativo (Serial/HID), UART (Serial1), I2C (SDA=D2, SCL=D3)

### 2. Joystick Analógico (KY‑023)
- **Pinos:** VRx → A0, VRy → A1, SW → pino digital (opcional)
- **Faixa analog:** 0–1023, com auto‑calibração dinâmica (mínimo/máximo)
- **Mapeamento:** valor central ajustado para 512, mapeado para movimentos de mouse

### 3. Botões Digitais
- **Botões de movimento:** esquerda (D9), cima (D10), direita (D8), baixo (D16)
- **Botão de clique:** btnX (D15)
- **Botão de modo:** btnComMode (D14) — pressionamento >3 s alterna entre HID e Bluetooth

### 4. Módulo Bluetooth MH‑10 (Bluetooth 2.0 SPP)
- **Alimentação:** 3.3–6 V (regulador onboard)
- **Níveis lógicos UART:** 3.3 V TTL
- **Baudrate padrão:** 9600 bps
- **PIN default:** 1234 (ou 0000)
- **Configuração AT:**
  - Entrar em modo AT → manter `KEY` em HIGH ao energizar (LED pisca lentamente)
  - Teste: `AT` → `OK`
  - Mudar nome: `AT+NAME<Nome>` (ex.: `AT+NAMECORE`) → `OK+Set:CORE`
  - Mudar PIN: `AT+PIN<6 dígitos>` (ex.: `AT+PIN123456`) → `OK+Set:123456`
  - Autenticação: `AT+TYPE2` → `OK+Set:2`

### 5. Carregador de Bateria 1 célula (TP4056 + BMS)
- **CI Carregador:** TP4056 (carrega Li‑ion 3.7 V até 4.2 V)
- **Proteção:** DW01A + FS8205A (sobretensão, subtensão, curto)
- **Entrada USB Micro:** 5 V, saída protegida B+ / B− para bateria
- **Saída protegida OUT+ / OUT−:** conecta ao step‑up ou ao Arduino RAW

### 6. Conversores Boost / Buck‑Boost
- **MT3608:** 2.0 V–24 V in → 5 V out ajustável, até 1 A
- **SX1308:** 2.0 V–24 V in → 5 V out ajustável, até 2 A
- **TPS61022:** 2.7 V–5.5 V in → 5 V out, até 2 A, eficiência ≤95%
- **Uso:** garantir 5 V estáveis ao alimentar o Arduino via VCC (fechar jumper J1)

---

## 💾 Software

### Arquitetura de Código
- **Modularização em funções:** `readJoystick()`, `moveMouse()`, `readButtons()`, `writeArrowButtons()`, `readMouseClick()`, `writeMouseClick()`, `checkToggle()`, `sendBluetoothJSON()`
- **Non‑blocking:** gerenciamento de clique de mouse e toggle sem `while` interno, usando `millis()` e flags
- **Alternância de modo:** pressionamento do botão de modo >3 s ativa/desativa `bluetoothEnabled`
- **Comunicação JSON:** via `Serial1` quando `bluetoothEnabled==true`:
  ```json
  {"x":-4,"y":5,"L":1,"U":0,"R":0,"D":1}
  ```

### Principais Funções
```cpp
void readJoystick(int &dx, int &dy);
void moveMouse(int x, int y);
void readButtons(bool &l, bool &u, bool &r, bool &d);
inline void writeArrowButtons(bool l, bool u, bool r, bool d);
inline void readMouseClick(bool &click);
inline void writeMouseClick(bool click);
inline void checkToggle();
inline void sendBluetoothJSON(int x, int y, bool l, bool u, bool r, bool d);
```

### Validação Bluetooth
- **Teste de eco AT:** sketch que envia `AT` para `Serial1` e ecoa resposta no `Serial` USB
- **Terminais:** PuTTY ou app móvel “Serial Bluetooth Terminal” a 9600 bps, pareando com PIN configurado

---

## 🔌 Conexões

| Sinal           | Módulo/Pin             |
|-----------------|------------------------|
| VRx             | Joystick → A0          |
| VRy             | Joystick → A1          |
| SW (opcional)   | Joystick → D2          |
| btnLeft         | D9 → GND quando LOW    |
| btnUp           | D10 → GND quando LOW   |
| btnRight        | D8 → GND quando LOW    |
| btnDown         | D16 → GND quando LOW   |
| btnX (clique)   | D15 → GND quando LOW   |
| btnToggle       | D14 → GND quando LOW   |
| MH‑10 TXD       | Pro Micro RX1 (D0)     |
| MH‑10 RXD       | Pro Micro TX1 (D1)     |
| TP4056 B+ / B−  | Bateria Li‑ion         |
| TP4056 OUT+ / − | Step‑up in / GND       |
| Step‑up out     | Arduino VCC (5 V)      |
| Step‑up GND     | Arduino GND            |

---

## 🚀 Uso

1. **Power up**: carregue a bateria (TP4056) e/ou conecte USB.
2. **Configurar Bluetooth (opcional)**:
   - Modo AT: segure KEY em HIGH e envie AT via Serial Monitor.
   - `AT+NAMECORE`, `AT+TYPE2`, `AT+PIN123456` → reinicie.
3. **Operação**:
   - **Modo HID** (padrão): movimenta mouse e seta teclas.
   - **Long‑press** btnToggle >3 s: alterna `bluetoothEnabled`.
   - **Modo JSON**: envia posições e botões via Bluetooth.
4. **Validar** via PuTTY (COM Bluetooth, 9600 bps) ou app móvel.

---

## 📄 Licença
MIT © Você

