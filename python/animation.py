import os
import sys
from open3d import *

def custom_draw_geometry(pcd):
	vis = Visualizer()
	vis.create_window()
	vis.add_geometry(pcd)
	vis.run()
	vis.destroy_window()

def custom_draw_geometry_with_rotation(pcd):
	def rotate_view(vis):
		opt = vis.get_render_option()
		opt.background_color = np.asarray([0, 0, 0])
		pcd.paint_uniform_color(np.asarray([255, 0, 0]))
		ctr = vis.get_view_control()
		ctr.rotate(10.0, 0.0)
		return False
	draw_geometries_with_animation_callback([pcd], rotate_view)

if len(sys.argv) != 2:
	exit()

pcd = read_point_cloud(sys.argv[1])
custom_draw_geometry_with_rotation(pcd)

