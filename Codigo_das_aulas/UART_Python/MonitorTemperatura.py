import serial
import serial.tools.list_ports
import threading
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

# Lista de baud rates comuns
BAUD_RATES = [9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600]

# Variáveis globais
ser = None
leitura_ativa = False

def listar_portas():
    portas = serial.tools.list_ports.comports()
    return [porta.device for porta in portas]

def decodificar_mensagem(dados):
    if len(dados) != 7:
        return None, None

    if dados[0] != 0x7F or dados[1] != 0xF0:
        return None, None

    tensao_mv = (dados[2] << 8) | dados[3]
    temp_escalada = (dados[4] << 8) | dados[5]
    checksum = sum(dados[:6]) & 0xFF

    if checksum != dados[6]:
        return None, None

    temperatura = ((temp_escalada / 65535.0) * 175.0) - 40.0
    return tensao_mv, temperatura

def ler_serial():
    global leitura_ativa
    buffer = []
    while leitura_ativa:
        if ser.in_waiting:
            byte = ser.read()
            if byte:
                buffer.append(ord(byte))
                if len(buffer) >= 7:
                    if buffer[0] == 0x7F and buffer[1] == 0xF0:
                        mensagem = buffer[:7]
                        buffer = buffer[7:]
                        tensao_mv, temperatura = decodificar_mensagem(mensagem)
                        if tensao_mv is not None:
                            atualizar_interface(tensao_mv, temperatura)
                    else:
                        buffer.pop(0)

def atualizar_interface(tensao, temperatura):
    lbl_tensao_valor.config(text=f"{tensao} mV")
    lbl_temp_valor.config(text=f"{temperatura:.2f} °C")

def conectar_serial():
    global ser, leitura_ativa
    porta = combo_porta.get()
    baud = int(combo_baud.get())

    if not porta:
        messagebox.showerror("Erro", "Selecione uma porta COM.")
        return

    try:
        ser = serial.Serial(porta, baud, timeout=1)
        leitura_ativa = True
        threading.Thread(target=ler_serial, daemon=True).start()
        btn_conectar.config(state='disabled')
        combo_porta.config(state='disabled')
        combo_baud.config(state='disabled')
    except Exception as e:
        messagebox.showerror("Erro ao conectar", str(e))

# Janela principal
janela = tk.Tk()
janela.title("Monitor Temperatura via UART – Prof Mozart")
janela.configure(bg="#555555")

# Título
tk.Label(janela, text="Monitor Temperatura via UART – Prof Mozart",
         font=("Arial", 18, "bold"), bg="#2e7d32", fg="white", pady=10).grid(row=0, column=0, columnspan=2, pady=10)

# Voltagem
tk.Label(janela, text="Voltagem do sensor", font=("Arial", 14),
         bg="#005f7f", fg="white", width=20, pady=10).grid(row=1, column=0, padx=20, pady=10)
lbl_tensao_valor = tk.Label(janela, text="Valor em milivolt", font=("Arial", 14),
                            bg="#005f7f", fg="white", width=20, pady=10)
lbl_tensao_valor.grid(row=1, column=1, padx=20, pady=10)

# Temperatura
tk.Label(janela, text="Temperatura Medida", font=("Arial", 14),
         bg="#005f7f", fg="white", width=20, pady=10).grid(row=2, column=0, padx=20, pady=10)
lbl_temp_valor = tk.Label(janela, text="Valor em graus Celsius", font=("Arial", 14),
                          bg="#005f7f", fg="white", width=20, pady=10)
lbl_temp_valor.grid(row=2, column=1, padx=20, pady=10)

# Combobox COM Port
tk.Label(janela, text="COM Port", font=("Arial", 12),
         bg="#005f7f", fg="white", width=15).grid(row=3, column=0, pady=10)
combo_porta = ttk.Combobox(janela, values=listar_portas(), width=15)
combo_porta.grid(row=3, column=1, pady=10)

# Combobox Baud Rate
tk.Label(janela, text="Baud Rate", font=("Arial", 12),
         bg="#005f7f", fg="white", width=15).grid(row=4, column=0, pady=10)
combo_baud = ttk.Combobox(janela, values=BAUD_RATES, width=15)
combo_baud.set("9600")
combo_baud.grid(row=4, column=1, pady=10)

# Botão conectar
btn_conectar = tk.Button(janela, text="Conectar", command=conectar_serial,
                         font=("Arial", 12), bg="#2e7d32", fg="white", width=20)
btn_conectar.grid(row=5, column=0, columnspan=2, pady=20)

janela.mainloop()