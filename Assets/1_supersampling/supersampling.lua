-- Reflective/refractive materials
water = gr.material({0.53, 0.69, 0.84}, {0.5, 0.5, 0.5}, 10, 0.5, 0.2, 1.33, 0.15)
glass = gr.material({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 10, 0.2, 0.6, 1.52, 0.1)	
disco_mat1 = gr.material({0.9, 0.9, 0.9}, {0.8, 0.8, 0.8}, 90, 0.8, 0.0, 1.65, 0)
mirror = gr.material({0.5, 0.5, 0.5}, {0.8, 0.8, 0.8}, 90, 1.0, 0.0, 1.0, 0)

red = gr.material({0.8, 0.2, 0.0}, {0.5, 0.7, 0.5}, 25)
gold = gr.material({0.9, 0.8, 0.6}, {0.8, 0.8, 0.8}, 55)
indigo = gr.material({0.0, 0.0, 0.6}, {0.3, 0.3, 0.8}, 25)
light_blue = gr.material({0.5, 0.8, 1.0}, {0.5, 0.4, 0.8}, 25)
pale = gr.material({0.9, 0.9, 1.0}, {0.3, 0.4, 0.3}, 10)

black = gr.material({0,0,0}, {0,0,0}, 0)
debug1 = gr.material({1,0,0}, {0,0,0}, 0)
debug2 = gr.material({0,1,0}, {0,0,0}, 0)
debug3 = gr.material({0,0,1}, {0,0,0}, 0)

scene = gr.node('scene')

cube = gr.cube('cube')
cube:scale(10, 10, 10)
cube:rotate('y', 45)
cube:rotate('x', 45)
cube:translate(0, 0, -100)
cube:set_material(red)
scene:add_child(cube)

left_light = gr.light({-8, 3, -10}, {0.4, 0.4, 0.4}, {1, 0.5, 0})
right_light = gr.light({8, 3, -10}, {0.4, 0.4, 0.4}, {1, 0.5, 0})
camera_light = gr.light({0, 0, 0}, {1.0, 1.0, 1.0}, {1, 0, 0})

gr.render(scene,'supersampling_off.png', 512, 512,
	  {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 45,
	  {0.5, 0.5, 0.5}, {camera_light, left_light, right_light})
