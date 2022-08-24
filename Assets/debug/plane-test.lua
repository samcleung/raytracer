black = {0,0,0}

red	 = gr.material({1.0, 0.0, 0.0}, black, 0, 0, 0, 1)
green   = gr.material({0.0, 1.0, 0.0}, black, 0, 0, 0, 1)
blue	= gr.material({0.0, 0.0, 1.0}, black, 0, 0, 0, 1)
magenta = gr.material({1.0, 0.0, 1.0}, black, 0, 0, 0, 1)


scene = gr.node( 'scene' )

bottom = gr.plane('bottom', {0, 0, 0}, {1, 0, 0}, {0, 0, -1}, 2)
bottom:translate(0, -1, 0)
scene:add_child(bottom)
bottom:set_material(red)

top = gr.plane('top', {0, 0, 0}, {1, 0, 0}, {0, 0, 1}, 2)
top:translate(0, 1, 0)
scene:add_child(top)
top:set_material(green)

back = gr.plane('back', {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, 2)
back:translate(0, 0, -5)
scene:add_child(back)
back:set_material(blue)

left = gr.plane('left', {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 2)
left:translate(-1, 0, 0)
scene:add_child(left)
left:set_material(magenta)

right = gr.plane('right', {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, 2)
right:translate(1, 0, 0)
scene:add_child(right)
right:set_material(magenta)

white_light = gr.light({0, 0, 0}, {0.8, 0.8, 0.8}, {1, 0, 0})

gr.render(scene,'plane.png', 512, 512,
	  {0, 0, 0}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light})
