import serial
import serial.tools.list_ports as list_ports;
import argparse
import sys
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

parser = argparse.ArgumentParser(description='Read and plot data from Movuino')
parser.add_argument('-l', '--list_ports', action='store_true', help='list all available serial ports')
parser.add_argument('-p', '--port', help='port where the movuino is connected')
parser.add_argument('-b', '--baudrate', default=115200, help='baudrate of the movuino Serial')
parser.add_argument('-f', '--output_file', default="spi_clock.csv", help='CSV file name')
parser.add_argument('-i', '--output_image', default="plot.png", help='plot file name')

args = parser.parse_args()

if args.list_ports:
    print('\n'.join([comport.device for comport in list_ports.comports()]))
    sys.exit(0)


movuino = serial.Serial(port=args.port, baudrate=args.baudrate, timeout=.1)
clk_freqs = []
success_rates = []

line = None
print("Waiting for Start order...")
while line != "START":
    line = movuino.readline().decode('utf-8').strip()
print("Recieved Start order")

while movuino.isOpen():
    raw_data = movuino.readline()
    if not len(raw_data):
        continue
    line = raw_data.decode('utf-8').strip()
    if line == "END":
        break

    clk_freq, success_rate = line.split(',')
    print(f"SPI Clock Freq: {clk_freq} Success rate: {success_rate}")
    clk_freqs.append(int(clk_freq))
    success_rates.append(int(success_rate))

with open(args.output_file, 'w') as f:
    f.write(f"clock_freq,success_rate\n")
    for freq, success in zip(clk_freqs, success_rates):
        f.write(f"{freq},{success}\n")

fig, ax = plt.subplots()
ax.set(xlabel='SPI Clock Freq (Hz)', ylabel='Success rate (%)',
       title='SPI connection success rate as a function of the SPI Clock frequency')
ax.grid()
plt.plot(clk_freqs, success_rates)
ax.yaxis.set_major_locator(MaxNLocator(10))
plt.savefig(args.output_image)
print(f"Data saved to {args.output_file}")
print(f"Plot saved to {args.output_image}")