# Spirit Sliders! 

Our program has 2 parts: the actual raytracer and a UI where you can explore different effects on an image 
and try them out. In our raytracer, we implemented soft shadows/area lights, lens assemblies, motion blur, 
and depth of view. The UI has 4 tabs: Motion Blur, Depth of Field, Lens Assemblies, and Sandbox. In the first 
three tabs, you can use different sliders and other buttons to explore what images using these effects our 
raytracer can produce. In the Sandbox tab, you can enter values for the effects and render your own scene. 
To use the Sandbox feature, you will need your own scenefile. To use the raytracer, add an .ini file in the 
command line with a path to the scenefile under IO/scene, a path to the lens .dat file under IO/lens, and 
a path to the output image under IO/output, and run the program. Lens files consist of a table with 4 columns 
representing the radius, index of refraction, thickness, and aperture of each lens interface. To use the UI, 
don't include a command line argument. 

We have no known bugs :)
