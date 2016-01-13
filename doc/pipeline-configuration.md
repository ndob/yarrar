# Pipeline configuration

## Example configuration
```
{
  "pipeline": {
    "dataproviders": [
      {
        "type": "static_image",
        "config": {
          "image_path": "../../test/fixture/img/multiple_markers.jpg"
        }
      }
    ],
    "trackers": [
      {
        "type": "marker",
        "config": {
          "parser": "yarrar_parser",
          "tracking_resolution_width": 320,
          "contour_min_area_size": 100,
          "inner_rectangle_min_size": 0.6,
          "marker_parser_image_size": 100,
          "binary_image_threshold": 100
        }
      }
    ],
    "renderers": [
      {
        "type": "opengl",
        "config": {}
      },
      {
        "type": "dummy",
        "config": {}
      }
    ]
  }
}
```

### Pipeline ("pipeline")

Valid children:

| Id            | Number of values  | Description |
| --------      | ------            | ------      |
| dataproviders | 1..*              | Array of data provider configurations |
| trackers      | 1..*              | Array of tracker configurations   |
| renderers     | 1..*              | Array of renderer configurations   |


### Dataproviders ("dataproviders")

Valid dataprovider configurations:

| Id            | Number of values  | Description |
| --------      | ------            | ------      |
| type          | 1                 | Type of provider. Valid values: static_image, webcam, android_image |
| config        | 1                 | Provider specific configuration   |

Provider specific configurations:

**static_image**

| Provider      | Number of values  | Description |
| --------      | ------            | ------      |
| image_path    | 1                 | Path to image |

### Trackers ("trackers")

Valid tracker configurations:

| Id            | Number of values  | Description |
| --------      | ------            | ------      |
| type          | 1                 | Type of tracker. Valid values: marker |
| config        | 1                 | Tracker specific configuration   |

Tracker specific configurations:

**marker**

| Provider      | Number of values  | Description |
| --------      | ------            | ------      |
| parser                    | 1                 | Marker data-field type. Valid values: yarrarparser |
| tracking_resolution_width | 1                 | Image resolution used in tracker. Valid values: integers |
| contour_min_area_size     | 1                 | Tracker internal configuration. Should be fine with default value |
| inner_rectangle_min_size  | 1                 | Tracker internal configuration. Should be fine with default value |
| marker_parser_image_size  | 1                 | Tracker internal configuration. Should be fine with default value |
| binary_image_threshold    | 1                 | Tracker internal configuration. Should be fine with default value |

Currently only *yarrarparser* data field parser is supported. Yarrarparser uses following kind of markers:

<img src=marker_id_30.png width=150 height=150 />
<img src=marker_id_112.png width=150 height=150 />

Printable markers:
* [Marker id 30](marker_id_30.png)
* [Marker id 112](marker_id_112.png)

### Renderers ("renderers")

Valid renderer configurations:

| Id            | Number of values  | Description |
| --------      | ------            | ------      |
| type          | 1                 | Type of renderer. Valid values: opengl, dummy, opencv |
| config        | 1                 | Renderer specific configuration   |
