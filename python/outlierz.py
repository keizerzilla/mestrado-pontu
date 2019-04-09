from open3d import *
import matplotlib.pyplot as plt

outstd = 0.1

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
		image = vis.capture_screen_float_buffer()
		plt.imshow(np.asarray(image))
		plt.show()
		return False
	def change_outlier(vis)
		print(outstd)
		outstd += 0.1
		outpcd, _ = statistical_outlier_removal(pcd, 5, outstd)
		vis.update_geometry(outpcd)
		print(outstd)
	key_to_callback = {}
	key_to_callback[ord("K")] = change_background_to_black
	key_to_callback[ord(",")] = capture_depth
	key_to_callback[ord(".")] = capture_image
	key_to_callback[ord("l")] = change_outlier
	draw_geometries_with_key_callbacks([pcd], key_to_callback)

pcd = read_point_cloud(sys.argv[1])
custom_draw_geometry_with_key_callback(pcd)

