## Project 3: Intersect

Please fill this out for Intersect only. The project handout can be found [here](https://cs1230.graphics/projects/ray/1).

### Output Comparison
Run the program with the specified `.ini` file to compare your output (it should automatically save to the correct path).
> If your program can't find certain files or you aren't seeing your output images appear, make sure to:<br/>
> 1. Set your working directory to the project directory
> 2. Set the command-line argument in Qt Creator to `template_inis/intersect/<ini_file_name>.ini`
> 3. Clone the `scenefiles` submodule. If you forgot to do this when initially cloning this repository, run `git submodule update --init --recursive` in the project directory

> Note: once all images are filled in, the images will be the same size in the expected and student outputs.

**I went through all the steps above and still do not have unit_cone_top.json or unit_cylinder_botton.json in my repository, so I am unable to compile those images**

| File/Method To Produce Output | Expected Output | Your Output |
| :---------------------------------------: | :--------------------------------------------------: | :-------------------------------------------------: |
| unit_cone.ini |  ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cone.png) | ![Place unit_cone.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cone.png) |
| unit_cone_cap.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cone_cap.png) | ![Place unit_cone_cap.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cone_cap.png) |
| unit_cone_top.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cone_top.png) | ![Place unit_cone_top.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cone_top.png) |
| unit_cube.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cube.png) | ![Place unit_cube.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cube.png) |
| unit_cylinder.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cylinder.png) | ![Place unit_cylinder.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cylinder.png) |
| unit_cylinder_bottom.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_cylinder_bottom.png) | ![Place unit_cylinder.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_cylinder_bottom.png) |
| unit_sphere.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/unit_sphere.png) | ![Place unit_sphere.png in student_outputs/intersect/required folder](student_outputs/intersect/required/unit_sphere.png) |
| parse_matrix.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/parse_matrix.png) | ![Place parse_matrix.png in student_outputs/intersect/required folder](student_outputs/intersect/required/parse_matrix.png) |
| ambient_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/ambient_total.png) | ![Place ambient_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/ambient_total.png) |
| diffuse_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/diffuse_total.png) | ![Place diffuse_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/diffuse_total.png) |
| specular_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/specular_total.png) | ![Place specular_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/specular_total.png) |
| phong_total.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/phong_total.png) | ![Place phong_total.png in student_outputs/intersect/required folder](student_outputs/intersect/required/phong_total.png) |
| directional_light_1.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/directional_light_1.png) | ![Place directional_light_1.png in student_outputs/intersect/required folder](student_outputs/intersect/required/directional_light_1.png) |
| directional_light_2.ini | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/intersect/required_outputs/directional_light_2.png) | ![Place directional_light_2.png in student_outputs/intersect/required folder](student_outputs/intersect/required/directional_light_2.png) |

### Design Choices

I decided to use polymorphism with this project for the shapes. The shape class has methods like calcNormal, calcIntersection, calcBoundingBox (for KdTree), and others. These methods get called when raytracing on whatever shape we are currently checking against. This was my most significant design choice. Others include delegating helper methods in the ray tracer scene and camera classes, and delegating to a bounding box struct and KdNode struct for my KdTree. I also chose to do adapative super sampling, where I continue sub sampling until the variance of the color in a neighborhood reaches below a certain threshold. 

### Collaboration/References

I used chatGPT for debugging help, although it didn't really do much besides catch low level syntax errors when setting up polymorphism. 

### Known Bugs

I have no known bugs in the basic funtionality of the program. My KdTree works, although does not speed everything up quickly enough as of right now. I was playing with the maxDepth of the recursion to see if that was my issue, but I never got to the bottom of it. It's likely something small, but as of right now my KdTree is setup and integrated with the program, but doesn't speed it up as much as I'd like. 

### Extra Credit

I completed the adaptive super sampling (found in the ray tracer class) and the KdTree (in the KdTree class). To go back to the original version of my project without these features, you can uncomment the method in the raytracer class and comment out everything below it. I have not included images of these features as they produce the same outputs as the normal ray tracer, just with faster speeds or more accuracy. The images generated above were done with the version of my project which implements both these features. 
