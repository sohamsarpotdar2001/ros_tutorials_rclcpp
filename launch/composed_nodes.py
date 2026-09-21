from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    cmd = ComposableNodeContainer(
        name='composed_nodes',
        namespace='',
        package='rclcpp_components',
        executable='component_container_mt',
        output='screen',
        composable_node_descriptions=[
            ComposableNode(
                package='ros_tutorials_rclcpp',
                plugin='ServiceNode',
                name='service_node',
            ),
            ComposableNode(
                package='ros_tutorials_rclcpp',
                plugin='ClientNode',
                name='client_node',
            ),
        ]
    )
   
    ld = LaunchDescription()
    ld.add_action(cmd)

    return ld

