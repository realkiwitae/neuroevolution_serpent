# Neuroevolution - Serpent

I decided to open an arena implementing a serpent like game where serpent corpse is a bundle of spheres, and the head can rotate in any directions.

The serpent pool will be about 20 units, I will modify the shader to include a "color" mask to hit which should reflect its fitness.

The serpents will aim to collect food which are uniformly distributed on the map, fixed number maybe sqrt of the map surface~ 
Once a food is eaten by a serpent, it will add 1 sphere to its body. an a new food will be spawned.

Wall collisions will be deadly, as for body collision with itselfs.

A serpent will die if he does not eat for 10s.

Every generation will breed new ones.
