Spacebox.
---------

Spacebox is a component which builds a cubemap texture of outer space. It 
automatically assigns the texture to your skybox.

Quick Start:
------------

Drag the SpaceBox prefab into your scene. Tweak the values in the StarField and
NebulaeField arrays.

How it Works:
-------------

The Spacebox component renders all its children to a cubemap, then assigns the
cubemap material to RenderSettings.skybox.

The StarField and NebulaeField settings procedurally create stars and nebulae
particles which are rendered then removed from the scene.

Star Settings:
-------------------

Particle Count: How many stars to generate.
Spread: How far apart the stars are spaced.
Distance: The distance at which the stars are generated.
Texture: The texture to use for the stars.
 star.

Nebulae Settings:
----------------------

As above except:
	
	Texture: base particle to use for the nebulae
	Ramp: Texture used to colorize the nebulae particles (See the Ramps! package) for
	an easy way to make more ramps. 