import sys
from open3d import *
import matplotlib.pyplot as plt

leafsize = 1
outstd = 1
pcd = read_point_cloud(sys.argv[1])

def custom_draw_geometry_with_key_callback(pcd):
	def change_background_to_black(vis):
		opt = vis.get_render_option()
		opt.background_color = np.asarray([0, 0, 0])
		return False
	def capture_depth(vis):
		depth = vis.capture_depth_float_buffer()
		plt.imshow(np.asarray(depth))
		plt.show()
		return False
	def capture_image(vis):
		print("eita")
		image = vis.capture_screen_float_buffer()
		plt.imshow(np.asarray(image))
		plt.show()
		return False
	def change_densit(vis):
		global leafsize
		global pcd
		leafsize += 0.1
		print(leafsize)
		voxelpcd = voxel_down_sample(pcd, 2)
		pcd.points = Vector3dVector(voxelpcd.points)
		vis.update_geometry()
		vis.poll_events()
		vis.update_renderer()
		return False
	def change_outlier(vis):
		global outstd
		global pcd
		outstd += 0.1
		print(outstd)
		outpcd, _ = statistical_outlier_removal(pcd, 40, 0.5)
		#outpcd, _ = radius_outlier_removal(pcd, 5, 10)
		pcd.points = Vector3dVector(outpcd.points)
		vis.update_geometry()
		vis.poll_events()
		vis.update_renderer()
		return False
	key_to_callback = {}
	key_to_callback[ord("K")] = change_background_to_black
	key_to_callback[ord(",")] = change_densit
	key_to_callback[ord(".")] = change_outlier
	draw_geometries_with_key_callbacks([pcd], key_to_callback)

custom_draw_geometry_with_key_callback(pcd)

