# ROS 2 Tutorials with RCLCPP

A ROS 2 C++ package containing examples and tutorial implementations built with [`rclcpp`](https://github.com/ros2/rclcpp), the ROS 2 Client Library for C++.

This repository is intended for learning the fundamentals of ROS 2 application development, including nodes, topics, services, and launch files.

## Features

- ROS 2 C++ nodes implemented with `rclcpp`
- Publisher and subscriber examples with turtlesim
- Service and client examples
- Custom service definitions
- Python launch files
- Composable Nodes
- Lifecycle nodes

## Repository Structure

```text
ros_tutorials_rclcpp/
├── launch/          # ROS 2 launch files
├── src/             # C++ node implementations
├── srv/             # Custom ROS 2 service definitions
├── CMakeLists.txt   # Package build configuration
├── package.xml      # Package metadata and dependencies
└── LICENSE          # Apache License 2.0
```

The package should be used with a ROS 2 distribution compatible with the dependencies declared in `package.xml`.

## Getting Started

### 1. Create a ROS 2 workspace

```
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
```

### 2. Clone the repository 
```
git clone https://github.com/sohamsarpotdar2001/ros_tutorials_rclcpp.git
```

### 3. Install dependencies
```
cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
sudo apt update && sudo apt install \
    ros-${ROS_DISTRO}-turtlebot3* \
    ros-${ROS_DISTRO}-turtlebot3-simulations
```

### 4. Build and source the package
```
colcon build --packages-select ros_tutorials_rclcpp
source install/setup.bash
```

## Running the examples
List the executables provided by the package
```
ros2 pkg executables ros_tutorials_rclcpp
```

Running an executable with
```
ros2 run ros_tutorials_rclcpp <executable_name>
```

Launch a launch file with
```
ros2 launch ros_tutorials_rclcpp <launch_file_name>
```

## Working with Servies
List the available service types
```
ros2 interface list | grep ros_tutorials_rclcpp
```

Inspect a custom service definition
```
ros2 interface show ros_tutorials_rclcpp/srv/<ServiceName>
```

List active services
```
ros2 service list
```

## Documentation
For more detailed tutorials, follow the Official ROS 2 [Documentation](https://docs.ros.org/en/jazzy/Tutorials.html)
