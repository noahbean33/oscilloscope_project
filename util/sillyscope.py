import sys

import serial
import threading

import matplotlib.animation as animation
import matplotlib.pyplot as plt

d_index = 0

fig, axs = plt.subplots(2)

ch1_data = [0] * 2048
ch2_data = [0] * 2048

x_data = [i for i in range(0, 2048)]

ch_update = threading.Event()
ch_lock = threading.Lock()

class CHPacket:
	def __init__(self):
		self.msg = 0
		self.ch = [0] * 4
		self.mlen = 0
		self.crc = 0

	def decode(self, bytestream):
		print(bytestream)
		self.msg = bytestream[1] << 8 | bytestream[0]
		self.mlen = bytestream[3] << 8 | bytestream[2]
		if self.mlen != 16:
			return False

		self.ch[0] = bytestream[5] << 8 | bytestream[4]
		self.ch[1] = bytestream[7] << 8 | bytestream[6]

		return True

	def get_channels(self):
		return self.ch

def serial_read_thread(ser_port):
	global ch1_data, ch2_data, ch_lock

	while True:
		chpacket = CHPacket()

		bytestream = bytearray()
		btype = ser_port.read(1)

		if btype == b'\x01':
			bytestream.extend(btype)
			bytestream.extend(ser_port.read(15))
			chpacket.decode(bytestream)

			ch_lock.acquire()

			ch1_data.append(chpacket.get_channels()[0])
			ch1_data = ch1_data[-2048:]

			ch2_data.append(chpacket.get_channels()[1])
			ch2_data = ch2_data[-2048:]

			ch_lock.release()

def run(i, ser_port):
	global ch1_data, ch2_data, ch_lock
	global axs, x_data

	ch_lock.acquire()

	axs[0].clear()
	axs[0].plot(x_data, ch1_data)

	axs[1].clear()
	axs[1].plot(x_data, ch2_data)

	ch_lock.release()

def main():

	ser_port = serial.Serial(
		port=sys.argv[1],
		baudrate=115200,
		bytesize=8,
		timeout=2,
		stopbits=serial.STOPBITS_ONE
	)

	t = threading.Thread(target=serial_read_thread, args=(ser_port,), daemon=True)
	t.start()

	anim = animation.FuncAnimation(fig, run, fargs=(ser_port,), interval=10)
	plt.show()

if __name__ == "__main__":
	main()
