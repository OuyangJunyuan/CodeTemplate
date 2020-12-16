# PCL 模板



## CMakeLists

```CMakeList
find_package(PCL 1.8 REQUIRED )
include_directories( ${PCL_INCLUDE_DIRS} )
message("123"${PCL_INCLUDE_DIRS})
add_definitions( ${PCL_DEFINITIONS} )
list(APPEND ALL_LIBS ${PCL_LIBRARIES})
```

## Header

```
#include <pcl/point_cloud.h>
#include <pcl/common/transforms.h>
#include <pcl/visualization/cloud_viewer.h>
```





## 可视化点云

```cpp
pcl::visualization::PCLVisualizer viewer ("Matrix transformation example");
   
pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> source_cloud_color_handler (cloud, 0, 0, 255);
viewer.addPointCloud (cloud,source_cloud_color_handler, "original_cloud");


pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> transform_cloud_color_handler (transformed_cloud, 255, 0, 0);
viewer.addPointCloud (transformed_cloud,transform_cloud_color_handler, "transform_cloud");
while (!viewer.wasStopped ()) { // 在按下 "q" 键之前一直会显示窗口
        viewer.spinOnce ();
}
```

