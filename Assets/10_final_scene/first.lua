-- Reflective/refractive materials
water = gr.material({0.53, 0.69, 0.84}, {0.5, 0.5, 0.5}, 10, 0.5, 0.2, 1.33, 0.15)
glass = gr.material({0.7, 0.7, 0.7}, {0.5, 0.5, 0.5}, 10, 0.2, 0.6, 1.52, 0.1)
glass_dark = gr.material({0.1, 0.1, 0.1}, {0.5, 0.5, 0.5}, 10, 0.2, 0.6, 1.52, 0.1)
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
back:translate(0, 0, -50)
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

-- disco ball
disco1_joint = gr.joint( 'disco1_joint', { 0, 0, 0 }, { -180, 0, 180 } )
disco1_ball = gr.disco_ball('disco1_ball', disco_mat1)
disco1_ball:scale(2.25, 2.25, 2.25)
disco1_joint:translate(0, 0, -20)
disco1_joint:add_child(disco1_ball)
scene:add_child(disco1_joint)

-- mirror 1
mirror_1 = gr.cube('mirror_1')
mirror_1:scale(5, 5, 0.2)
mirror_1:rotate('y', 90)
mirror_1:translate(-8, 1.5, -32)
scene:add_child(mirror_1)
mirror_1:set_material(mirror)

-- mirror 2
mirror_2 = gr.cube('mirror_2')
mirror_2:scale(4, 4, 0.2)
mirror_2:rotate('y', 90)
mirror_2:translate(-8, 1.5, -23)
scene:add_child(mirror_2)
mirror_2:set_material(mirror)

-- mirror 3
mirror_3 = gr.cube('mirror_3')
mirror_3:scale(3, 3, 0.2)
mirror_3:rotate('y', 90)
mirror_3:translate(-8, 1.5, -15)
scene:add_child(mirror_3)
mirror_3:set_material(mirror)

-- mirror 4
mirror_4 = gr.cube('mirror_4')
mirror_4:scale(2, 2, 0.2)
mirror_4:rotate('y', 90)
mirror_4:translate(-8, 1.5, -8)
scene:add_child(mirror_4)
mirror_4:set_material(mirror)

-- mirror 5
mirror_5 = gr.cube('mirror_5')
mirror_5:scale(1, 1, 0.2)
mirror_5:rotate('y', 90)
mirror_5:translate(-8, 1.5, -2)
scene:add_child(mirror_5)
mirror_5:set_material(mirror)

-- right window pane 1
right_window_1 = gr.cube('right_window_1')
right_window_1:scale(8, 3, 1.25)
right_window_1:rotate('y', -90)
right_window_1:translate(8, 1, -5)
scene:add_child(right_window_1)
right_window_1:set_material(glass)

-- right window pane 2
right_window_2 = gr.cube('right_window_2')
right_window_2:scale(7, 3, 1)
right_window_2:rotate('y', -90)
right_window_2:translate(8, 1, -20)
scene:add_child(right_window_2)
right_window_2:set_material(glass)

-- right window pane 3
right_window_3 = gr.cube('right_window_3')
right_window_3:scale(6, 3, 0.75)
right_window_3:rotate('y', -90)
right_window_3:translate(8, 1, -35)
scene:add_child(right_window_3)
right_window_3:set_material(glass_dark)

-- coil
coil_cyl = gr.cylinder('coil_cyl')
coil_cyl:scale(0.1, 5, 0.1)
coil_cyl:translate(0, 5, -20)
scene:add_child(coil_cyl)
coil_cyl:set_material(glass)

left_light = gr.light({-8, 3, -10}, {0.2, 0.2, 0.2}, {1, 0.5, 0})
right_light = gr.light({8, 3, -10}, {0.2, 0.2, 0.2}, {1, 0.5, 0})
camera_light = gr.area_light(
	{0, 0, 0}, {1.0, 1.0, 1.0}, {1, 0, 0},
	{-1, 0, 0}, {0, 1, 0}, 0.5)

-- gr.render(scene,'first.png', 512, 512,
-- 	  {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 45,
-- 	  {0.5, 0.5, 0.5}, {camera_light, left_light, right_light})

gr.animate(scene,'third', 256, 256,
	{0, 0, -18}, {0, 0, -1}, {0, 1, 0}, 45,
	{0.5, 0.5, 0.5}, {camera_light, left_light, right_light},
	{24, 1},
	1);