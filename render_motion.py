import json
import os

# Base scene template
base_scene = {
  "name": "root",
  "globalData": {
    "ambientCoeff": 0.5,
    "diffuseCoeff": 0.5,
    "specularCoeff": 0.5,
    "transparentCoeff": 0,
    "globalVel": 0.0
  },
  "cameraData": {
    "position": [
      10.0,
      10.0,
      10.0
    ],
    "up": [
      0.0,
      1.0,
      0.0
    ],
    "heightAngle": 30.0,
    "focus": [
      0.0,
      0.0,
      0.0
    ],
    "aperture": 0.5,
    "focalLength": 9.0
  },
  "groups": [
     { "translate": [10, 80, 30],
            "lights": [
              {
                "type": "area",
                "color": [
                  10,
                  10,
                  10
                ],
                "direction": [
                  0,
                  1,
                  0
                ],
                "width": 20.0,
                "height": 20.0,
                "attenuationCoeff": [0.1, 0.1, 0.0]
              }
            ]
          },
     { "translate": [10, 30,90],
            "lights": [
              {
                "type": "area",
                "color": [
                  5,
                  5,
                  5
                ],
                "direction": [
                  0,
                  1,
                  0
                ],
                "width": 20.0,
                "height": 20.0,
                "attenuationCoeff": [0.3, 0.3, 0.0]
              }
            ]
          },
    {
      "groups": [
          {"translate": [
              -5, 2, -5
            ],
              "rotate": [
                  0,
                  1,
                  0,
                  45
              ],
            "scale": [
              20,
              7,
              7
            ],
            "primitives": [
              {
                "type": "cube",
                "diffuse": [
                  0.0,
                  0.0,
                  1.0
                ],
                "specular": [
                  1.0,
                  1.0,
                  1.0
                ],
                "shininess": 0,
                "blend": 1.0,
                "textureFile": "textures/sunset.jpg",
                "textureU": 1.0,
                "textureV": 1.0,
                "velocity": [
                  0.0,
                  0.0,
                  0.0
                ]
               
              }
            ]
          },
          {"translate": [
              -6, -15, -8
            ],
              "rotate": [
                  -0.3,
                  1,
                  0.15,
                  45
              ],
            "scale": [
              40,
              7,
              30
            ],
            "primitives": [
              {
                "type": "cube",
                "diffuse": [
                  0.0,
                  1.0,
                  0.0
                ],
                "specular": [
                  1.0,
                  1.0,
                  1.0
                ],
                "shininess": 0,
                "blend": 0.5,
                "textureFile": "textures/field.jpg",
                "textureU": 1.0,
                "textureV": 1.0,
                "velocity": [
                  0.0,
                  0.0,
                  0.0
                ]
               
              }
            ]
          },
        {
          "translate": [
              -4.5,
              -3,
              1.0
          ],
          "rotate": [
            -0.75,
            4,
            -4,
            120
          ],
          "scale": [
            4.5,
            4.5,
            4.5
          ],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [
                1.0,
                0.55,
                0.0
              ],
              "specular": [
                1.0,
                1.0,
                1.0
              ],
              "shininess": 15,
              "blend": 1.0,
              "textureFile": "textures/basketball.jpg",
              "textureU": 1.0,
              "textureV": 1.0,
              "velocity": [
                0.0,
                1.04,
                0.0
              ]
            }
          ]
        },
        {
          "translate": [
              0.45,
              2.0,
              0.0
          ],
          "rotate": [
            -0.35,
            1,
            0,
            120
          ],
          "scale": [
            2.25,
            2.25,
            2.25
          ],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [
                1.0,
                1.0,
                1.0
              ],
              "specular": [
                1.0,
                1.0,
                1.0
              ],
              "shininess": 20,
              "blend": 1.0,
              "textureFile": "textures/volleyball2.jpg",
              "textureU": 1.0,
              "textureV": 1.0,
              "velocity": [
                0.0,
                1.12,
                0.0
              ]
            }
          ]
        },
        {
          "translate": [
              -4.0,
              2.0,
              2.0
          ],
          "rotate": [
            0.5,
            0.73,
            1,
            100
          ],
          "scale": [
            1.1,
            1.1,
            1.1
          ],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [
                0.76,
                0.84,
                0.52
              ],
              "specular": [
                1.0,
                1.0,
                1.0
              ],
              "shininess": 25,
              "blend": 1.0,
              "textureFile": "textures/tennis.jpg",
              "textureU": 1.0,
              "textureV": 1.0,
              "velocity": [
                0.0,
                1.2,
                0.0
              ]
            }
          ]
        },
        {
          "translate": [
            3.5,
            0.1,
            2.0
          ],
          "rotate": [
            0.3,
            1.45,
            2.0,
            100
          ],
          "scale": [
            1.2,
            1.2,
            1.2
          ],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [
                1.0,
                1.0,
                1.0
              ],
              "specular": [
                1.0,
                1.0,
                1.0
              ],
              "shininess": 10,
              "blend": 1.0,
              "textureFile": "textures/baseball.jpg",
              "textureU": 2.0,
              "textureV": 1.0,
              "velocity": [
                0.0,
                1.16,
                0.0
              ]
            }
          ]
        },
        {
          "translate": [
            2.75,
        -0.3,
            -3.0
          ],
          "rotate": [
            0.4,
            0.65,
            1.5,
            100
          ],
          "scale": [
            3.0,
            3.0,
            3.0
          ],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [
                1.0,
                1.0,
                1.0
              ],
              "specular": [
                1.0,
                1.0,
                1.0
              ],
              "shininess": 15,
              "blend": 1.0,
              "textureFile": "textures/soccer.jpg",
              "textureU": 1.6,
              "textureV": 1.0,
              "velocity": [
                0.0,
                1.08,
                0.0
              ]
            }
          ]
        }
      ]
    }
  ]
}

ini_template = """[IO]
    scene = {scene_file}
    output = {output_file}

[Canvas]
    width = 1024
    height = 768

[Feature]
    shadows = false
    reflect = false
    refract = false
    texture = false
    parallel = false
    super-sample = false
    num-samples = 1
    post-process = false
    acceleration = false
    depthoffield = false
    motion-blur = true

"""

scene_output_dir = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/scenefiles/falling_spheres"
ini_output_dir = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/inifiles/falling_spheres"
output_dir_for_ini = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/outputs/falling_spheres"

# Ensure directories exist
os.makedirs(scene_output_dir, exist_ok=True)
os.makedirs(ini_output_dir, exist_ok=True)
os.makedirs(output_dir_for_ini, exist_ok=True)  # This is for consistency if the path is used later

# Generate 100 files with varying aperture settings
for i in range(101):
    # Update the scene file
    scene = base_scene.copy()
    global_vel_value = i * 0.03
    scene["globalData"]["globalVel"] = global_vel_value
    scene_file_name = os.path.join(scene_output_dir, f"scene_{i + 1}.json")
    with open(scene_file_name, "w") as f:
        json.dump(scene, f, indent=2)

    # Create the corresponding .ini file
    output_file_path = os.path.join(output_dir_for_ini, f"output_{i + 1}.png")
    ini_content = ini_template.format(
        scene_file=scene_file_name,
        output_file=output_file_path
    )
    ini_file_name = os.path.join(ini_output_dir, f"scene_{i + 1}.ini")
    with open(ini_file_name, "w") as f:
        f.write(ini_content)

print(f"20 scene and ini files generated in '{scene_output_dir}' and '{ini_output_dir}'")
