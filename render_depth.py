import json
import os

# Base scene template
base_scene = {
  "name": "root",
  "globalData": {
    "ambientCoeff": 0.5,
    "diffuseCoeff": 0.5,
    "specularCoeff": 0.5,
    "transparentCoeff": 0
  },
  "cameraData": {
    "position": [8.0, 8.0, 8.0],
    "up": [0.0, 1.0, 0.0],
    "heightAngle": 30.0,
    "focus": [0.0, 0.0, 0.0],
    "aperture": 0.5,
    "focalLength": 8.0
  },
  "groups": [
    {
      "lights": [
        {
          "type": "directional",
          "color": [0.71, 0.68, 0.91],
          "direction": [2, -3, -3]
        },
        {
          "type": "directional",
          "color": [0.95, 0.59, 0.28],
          "direction": [3, -3, -4]
        },
        {
          "type": "directional",
          "color": [0.71, 0.83, 0.6],
          "direction": [2, -4, 3]
        },
        {
          "type": "directional",
          "color": [0.48, 0.55, 0.55],
          "direction": [0, -2, 3]
        },
        {
          "type": "directional",
          "color": [0.52, 0.39, 0.79],
          "direction": [-3, 0, -3]
        }
      ]
    },
    {
      "groups": [
        {
          "translate": [2.34, -0.45, -2.54],
          "scale": [0.81, 0.81, 0.81],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.41, 0.83, 0.74],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 22
            }
          ]
        },
        {
          "translate": [-0.05, 1.73, -2.63],
          "scale": [1.11, 1.11, 1.11],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.85, 0.35, 0.13],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 19
            }
          ]
        },
        {
          "translate": [0.63, 0.07, 2.73],
          "rotate": [1, 1, 0, 28.00773350985932],
          "scale": [1.01, 1.01, 1.01],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.47, 0.22, 0.09],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 29
            }
          ]
        },
        {
          "translate": [0.62, 1.96, -2.3],
          "rotate": [0, 0, 1, 4.381434328751382],
          "scale": [1.37, 1.37, 1.37],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.14, 0.16, 0.05],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 25
            }
          ]
        },
        {
          "translate": [-2.53, -1.33, -2.09],
          "rotate": [1, 0, 0, -51.014660889229134],
          "scale": [0.49, 0.49, 0.49],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.54, 0.11, 0.28],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 23
            }
          ]
        },
        {
          "translate": [1.9, 0.51, 2.41],
          "rotate": [0, 1, 0, 52.63048508278186],
          "scale": [0.75, 0.75, 0.75],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.51, 0.52, 0.65],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 28
            }
          ]
        },
        {
          "translate": [0.95, -1.63, 0.2],
          "rotate": [0, 1, 1, 7.327781719722367],
          "scale": [1.8, 1.8, 1.8],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.83, 0.24, 0.72],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 25
            }
          ]
        },
        {
          "translate": [-2.4, 1.44, -0.34],
          "rotate": [0, 1, 0, -51.29988243644887],
          "scale": [0.56, 0.56, 0.56],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.29, 0.35, 0.64],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [-1.04, 0.96, -0.42],
          "rotate": [0, 0, 1, 79.06970185001867],
          "scale": [1.69, 1.69, 1.69],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.06, 0.04, 0.51],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 19
            }
          ]
        },
        {
          "translate": [0.36, -1.93, -1.57],
          "rotate": [1, 0, 0, 1.0021802582755015],
          "scale": [0.64, 0.64, 0.64],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.92, 0.2, 0.69],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 12
            }
          ]
        },
        {
          "translate": [-2.75, 0.65, 1.24],
          "rotate": [0, 0, 1, 39.03019955902752],
          "scale": [0.75, 0.75, 0.75],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.95, 0.16, 0.65],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 27
            }
          ]
        },
        {
          "translate": [-2.5, -1.42, 1.09],
          "rotate": [1, 0, 0, 84.48895611109992],
          "scale": [0.42, 0.42, 0.42],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.59, 0.46, 0.33],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 17
            }
          ]
        },
        {
          "translate": [-2.31, -0.24, 2.24],
          "rotate": [1, 1, 0, 76.6835852831685],
          "scale": [0.63, 0.63, 0.63],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.06, 0.29, 0.18],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 27
            }
          ]
        },
        {
          "translate": [-0.03, -0.7, -0.04],
          "rotate": [1, 1, 1, 25.496293747039957],
          "scale": [1.16, 1.16, 1.16],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.72, 0.16, 0.47],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 26
            }
          ]
        },
        {
          "translate": [0.64, 2.23, -2.49],
          "rotate": [1, 1, 1, -0.0014901265085001114],
          "scale": [1.73, 1.73, 1.73],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.0, 0.81, 0.02],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [-0.29, -1.85, -2.2],
          "rotate": [1, 1, 1, -34.978604471587296],
          "scale": [1.69, 1.69, 1.69],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.4, 0.87, 0.35],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 28
            }
          ]
        },
        {
          "translate": [2.91, -0.95, 0.95],
          "rotate": [0, 1, 1, -48.835905273003924],
          "scale": [1.62, 1.62, 1.62],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.65, 0.02, 0.56],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 21
            }
          ]
        },
        {
          "translate": [-0.9, -1.58, -2.08],
          "rotate": [0, 1, 1, 60.59282079174807],
          "scale": [0.88, 0.88, 0.88],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.11, 0.29, 0.83],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [1.52, -2.7, -2.25],
          "rotate": [0, 1, 0, -75.52227980196211],
          "scale": [1.17, 1.17, 1.17],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.66, 0.45, 0.38],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 23
            }
          ]
        },
        {
          "translate": [-2.3, -1.53, 1.8],
          "rotate": [1, 1, 1, 39.59581747017549],
          "scale": [0.76, 0.76, 0.76],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.2, 0.86, 0.2],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 15
            }
          ]
        },
        {
          "translate": [-2.24, -2.73, -2.22],
          "rotate": [1, 1, 0, -46.992046741550254],
          "scale": [1.49, 1.49, 1.49],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.33, 0.37, 0.55],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 12
            }
          ]
        },
        {
          "translate": [-2.22, -0.16, 0.74],
          "rotate": [0, 1, 1, 50.33731994845908],
          "scale": [1.1, 1.1, 1.1],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.87, 0.05, 0.5],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 24
            }
          ]
        },
        {
          "translate": [-0.93, 0.89, 1.08],
          "rotate": [1, 0, 1, 9.438539620058734],
          "scale": [1.1, 1.1, 1.1],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.74, 0.02, 0.04],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [1.13, -2.11, -2.76],
          "rotate": [1, 1, 1, -58.32652676714815],
          "scale": [0.76, 0.76, 0.76],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.51, 0.09, 0.14],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 10
            }
          ]
        },
        {
          "translate": [-1.8, -2.81, 1.94],
          "rotate": [1, 1, 0, -32.28708497598],
          "scale": [1.38, 1.38, 1.38],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.45, 0.39, 0.93],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 19
            }
          ]
        },
        {
          "translate": [-2.22, -0.74, -1.36],
          "rotate": [1, 1, 0, 56.13202518347339],
          "scale": [1.02, 1.02, 1.02],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.91, 0.75, 0.65],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 25
            }
          ]
        },
        {
          "translate": [-2.17, -0.03, -0.07],
          "scale": [0.79, 0.79, 0.79],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.38, 0.45, 0.06],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 10
            }
          ]
        },
        {
          "translate": [1.32, -0.64, -2.92],
          "rotate": [0, 1, 1, 55.008026061912915],
          "scale": [1.15, 1.15, 1.15],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.74, 0.64, 0.02],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 22
            }
          ]
        },
        {
          "translate": [0.46, -1.3, 0.16],
          "scale": [1.29, 1.29, 1.29],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.19, 0.57, 0.24],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 26
            }
          ]
        },
        {
          "translate": [0.4, -2.04, -1.42],
          "rotate": [0, 1, 1, -4.408229225430219],
          "scale": [1.01, 1.01, 1.01],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.51, 0.48, 0.72],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [2.26, -1.34, 2.06],
          "rotate": [0, 1, 0, -69.71467779559389],
          "scale": [1.73, 1.73, 1.73],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.37, 0.05, 0.43],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [-0.57, -2.32, 1.84],
          "rotate": [1, 0, 1, 18.800376034344637],
          "scale": [0.84, 0.84, 0.84],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.51, 0.84, 0.59],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 22
            }
          ]
        },
        {
          "translate": [-1.08, -1.33, 1.95],
          "rotate": [1, 0, 0, -21.920372811155687],
          "scale": [1.01, 1.01, 1.01],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.61, 0.37, 0.34],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 20
            }
          ]
        },
        {
          "translate": [2.02, -0.7, -1.55],
          "rotate": [1, 1, 0, 62.82573153663873],
          "scale": [1.7, 1.7, 1.7],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.6, 0.94, 0.8],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 10
            }
          ]
        },
        {
          "translate": [-0.16, 0.55, 2.14],
          "rotate": [1, 1, 0, 64.68327964376135],
          "scale": [1.43, 1.43, 1.43],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.85, 0.85, 0.73],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 11
            }
          ]
        },
        {
          "translate": [2.2, -0.78, -2.12],
          "rotate": [1, 0, 0, -38.111576279980945],
          "scale": [0.82, 0.82, 0.82],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.04, 0.93, 0.59],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 21
            }
          ]
        },
        {
          "translate": [2.1, 2.87, -0.97],
          "rotate": [0, 1, 0, 83.72055491722077],
          "scale": [1.69, 1.69, 1.69],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.13, 0.5, 0.21],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 28
            }
          ]
        },
        {
          "translate": [-2.46, -2.13, -2.92],
          "rotate": [1, 0, 1, 44.97405166741126],
          "scale": [1.26, 1.26, 1.26],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.25, 0.9, 0.25],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 17
            }
          ]
        },
        {
          "translate": [1.2, 2.67, -2.52],
          "scale": [0.94, 0.94, 0.94],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.38, 0.16, 0.12],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 19
            }
          ]
        },
        {
          "translate": [-1.83, -1.02, 2.99],
          "rotate": [1, 1, 0, 15.624253723976864],
          "scale": [1.63, 1.63, 1.63],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.53, 0.82, 0.36],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 30
            }
          ]
        },
        {
          "translate": [2.18, -2.72, -2.83],
          "rotate": [1, 0, 1, 84.16616282524575],
          "scale": [0.62, 0.62, 0.62],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.56, 0.95, 0.84],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 18
            }
          ]
        },
        {
          "translate": [2.42, 0.57, -0.91],
          "rotate": [0, 0, 1, 71.21273508013942],
          "scale": [0.57, 0.57, 0.57],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.17, 0.76, 0.08],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 11
            }
          ]
        },
        {
          "translate": [-0.01, -1.75, 0.62],
          "rotate": [0, 0, 1, -64.6607759571312],
          "scale": [0.85, 0.85, 0.85],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.46, 0.92, 0.99],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 26
            }
          ]
        },
        {
          "translate": [1.44, 0.69, 1.28],
          "rotate": [1, 1, 0, -17.783403998276583],
          "scale": [0.87, 0.87, 0.87],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.37, 0.03, 0.59],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 22
            }
          ]
        },
        {
          "translate": [-1.41, -0.04, -0.78],
          "rotate": [0, 1, 0, -63.665838240348705],
          "scale": [1.6, 1.6, 1.6],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.1, 0.09, 0.62],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 22
            }
          ]
        },
        {
          "translate": [0.11, 0.37, -1.17],
          "rotate": [1, 1, 0, -51.58073857601467],
          "scale": [1.0, 1.0, 1.0],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.92, 0.69, 0.81],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 24
            }
          ]
        },
        {
          "translate": [-0.84, -2.64, -0.49],
          "rotate": [0, 1, 1, 4.184867362537332],
          "scale": [0.56, 0.56, 0.56],
          "primitives": [
            {
              "type": "sphere",
              "diffuse": [0.82, 0.64, 0.55],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 10
            }
          ]
        },
        {
          "translate": [-0.97, -0.34, -2.88],
          "rotate": [1, 1, 0, 88.69075434800624],
          "scale": [1.67, 1.67, 1.67],
          "primitives": [
            {
              "type": "cylinder",
              "diffuse": [0.35, 0.87, 0.15],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 13
            }
          ]
        },
        {
          "translate": [-2.53, -1.5, -2.55],
          "rotate": [1, 1, 0, 70.23452302542637],
          "scale": [0.56, 0.56, 0.56],
          "primitives": [
            {
              "type": "cube",
              "diffuse": [0.29, 0.19, 0.84],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 18
            }
          ]
        },
        {
          "translate": [-2.95, 1.45, -1.75],
          "rotate": [1, 0, 1, 26.839564290056117],
          "scale": [0.99, 0.99, 0.99],
          "primitives": [
            {
              "type": "cone",
              "diffuse": [0.59, 0.69, 0.45],
              "specular": [1.0, 1.0, 1.0],
              "shininess": 27
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
depthoffield = true
"""

# motion = false
# lens = false

scene_output_dir = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/scenefiles/primsalad"
ini_output_dir = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/inifiles/primsalad"
output_dir_for_ini = "/Users/efratavigdor/Desktop/CS1230/graphics-final-project/outputs/primsalad"

# Ensure directories exist
os.makedirs(scene_output_dir, exist_ok=True)
os.makedirs(ini_output_dir, exist_ok=True)
os.makedirs(output_dir_for_ini, exist_ok=True)  # This is for consistency if the path is used later

# Generate 100 files with varying aperture settings
for i in range(20):
    # Update the scene file
    scene = base_scene.copy()
    scene["cameraData"]["focalLength"] = 9 + (4 / 20) * i  # Linearly interpolate from 8 to 15
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
