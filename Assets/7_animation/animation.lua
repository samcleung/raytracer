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

sea_floor = gr.cube('floor')
sea_floor:scale(25, 1, 100)
sea_floor:translate(-10, -10, 0)
sea_floor:set_material(light_blue)
scene:add_child(sea_floor)

bottom = gr.water_plane('bottom', {0, 0, 0}, {60, 0, 0}, {0, 0, -60}, water)
bottom:translate(-10, -7, 0)
bottom:rotate('y', 45)
bottom:set_material(water)
scene:add_child(bottom)

top = gr.plane('top', {0, 0, 0}, {1, 0, 0}, {0, 0, 1}, 2)
top:translate(0, 10, 0)
scene:add_child(top)
top:set_material(gold)

front = gr.plane('front', {0, 0, 0}, {-1, 0, 0}, {0, 1, 0}, 2)
front:translate(0, 0, 10)
scene:add_child(front)
front:set_material(pale)

back = gr.plane('back', {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, 2)
back:translate(0, 0, -60)
scene:add_child(back)
back:set_material(gold)

left = gr.plane('left', {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 2)
left:translate(-10, 0, 0)
scene:add_child(left)
left:set_material(indigo)

right = gr.plane('right', {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, 2)
right:translate(10, 0, 0)
scene:add_child(right)
right:set_material(light_blue)

left_light = gr.light({-8, 3, -10}, {0.4, 0.4, 0.4}, {1, 0.5, 0})
right_light = gr.light({8, 3, -10}, {0.4, 0.4, 0.4}, {1, 0.5, 0})
camera_light = gr.area_light(
	{0, 0, 0}, {1.0, 1.0, 1.0}, {1, 0, 0},
	{-1, 0, 0}, {0, 1, 0}, 1.25)

-- gr.render(scene,'first.png', 1024, 1024,
-- 	  {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 45,
-- 	  {0.5, 0.5, 0.5}, {camera_light, left_light, right_light})

gr.animate(scene,'animation_reflections', 1024, 1024,
	{0, 0, -15}, {0, 0, -1}, {0, 1, 0}, 45,
	{0.5, 0.5, 0.5}, {camera_light, left_light, right_light},
	{24, 2},
	1);