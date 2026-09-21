cmake_minimum_required(VERSION 3.8)
project(ros_tutorials_rclcpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
    "srv/AverageVel.srv"
    DEPENDENCIES turtlesim
)

set(dependencies
    rclcpp
    rclcpp_components
    std_msgs
    geometry_msgs
    sensor_msgs
    turtlesim
    rclcpp_action
)

add_executable(publisher_node src/publisher_class.cpp)
ament_target_dependencies(publisher_node ${dependencies})

add_executable(publisher src/publisher.cpp)
ament_target_dependencies(publisher ${dependencies})

add_executable(subscriber_node src/subscriber_class.cpp)
ament_target_dependencies(subscriber_node ${dependencies})

add_executable(turtle_square src/turtle_square.cpp)
ament_target_dependencies(turtle_square ${dependencies})

add_executable(turtlebot_obs_avoid src/turtlebot_obs_avoid.cpp)
ament_target_dependencies(turtlebot_obs_avoid ${dependencies})

add_executable(turtle_spawn src/turtle_spawn.cpp)
ament_target_dependencies(turtle_spawn ${dependencies})

add_executable(lifecycle_talker src/lifecycle_talker.cpp)
ament_target_dependencies(lifecycle_talker 
    "rclcpp_lifecycle"
    "lifecycle_msgs" 
    "rclcpp" 
    "std_msgs"
)

add_executable(lifecycle_listener src/lifecycle_listener.cpp)
ament_target_dependencies(lifecycle_listener
  "lifecycle_msgs"
  "rclcpp"
  "std_msgs"
)

add_executable(lifecycle_service_client src/lifecycle_service_client.cpp)
ament_target_dependencies(lifecycle_service_client
  "lifecycle_msgs"
  "rclcpp"
)

add_library(composition_nodes SHARED
            src/composed_service.cpp
            src/composed_client.cpp
)
ament_target_dependencies(composition_nodes ${dependencies})

rclcpp_components_register_nodes(composition_nodes "ServiceNode")
rclcpp_components_register_nodes(composition_nodes "ClientNode")

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(DIRECTORY launch
	DESTINATION share/${PROJECT_NAME}
)

install(TARGETS
    publisher_node
    publisher
    subscriber_node
    turtle_square
    turtle_spawn
    turtlebot_obs_avoid
    lifecycle_talker
    lifecycle_listener
    lifecycle_service_client
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib/${PROJECT_NAME}
)

ament_export_targets(export_composition_nodes)
install(TARGETS
    composition_nodes
    EXPORT export_composition_nodes
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)

target_link_libraries(composition_nodes "${cpp_typesupport_target}")

if(BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    ament_lint_auto_find_test_dependencies()
endif()

ament_package()
