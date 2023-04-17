#!/usr/bin/env python3
import timeit
import json
import random
import matplotlib

MEM_SIZE = 9900
ROWS = 100
COLS = 99
SQUARE_SPACING = 7

SCREEN_WIDTH = int(COLS * SQUARE_SPACING) + 20
SCREEN_HEIGHT = int(ROWS * SQUARE_SPACING) + 20
SCREEN_TITLE = "Arena"


class Consts(object):
	def __init__(self, rows, cols, my_ch, opp_ch):
		self.rows = rows
		self.cols = cols
		self.size = rows * cols
		self.my_ch = my_ch
		self.opp_ch = opp_ch
		self.height = 0
		self.width = 0
		self.sq_spacing = 7
		self.half_sq = 3




def main():

	with open('../vis.json') as f:
		data = json.load(f)
	# const = Consts(data[0]['Consts']['map_rows'],
	# 	               data[0]['Consts']['map_cols'],
	# 	               data[0]['Consts']['my_ch'],
	# 	               data[0]['Consts']['opp_ch'])

	sq_spac_y = int(round(700 / const.rows))
	sq_spac_x = int(round(700 / const.cols))

	const.width = 700
	const.height = 700

	if sq_spac_x < sq_spac_y:
		const.sq_spacing = sq_spac_x
	else:
		const.sq_spacing = sq_spac_y

	if const.sq_spacing % 2 == 0:
		const.half_sq = int((const.sq_spacing- 2) / 2)
	else:
		const.half_sq = int((const.sq_spacing - 1) / 2)

	const.height = const.sq_spacing * const.rows +20
	const.width = const.sq_spacing * const.cols + 20

	print("WIDTH =")
	print(const.width)
	print("HEIGHT:")
	print(const.height)
	print("sq_spacing")
	print(const.sq_spacing)
	print("half_sq")
	print(const.half_sq)



if __name__ == "__main__":
	main()
