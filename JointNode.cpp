#include "JointNode.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "cs488-framework/MathUtils.hpp"

using namespace glm;

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name), m_joint_angle_x(0.0f), m_joint_angle_y(0.0f)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_y_range.min = min;
	m_joint_x_range.init = init;
	m_joint_x_range.max = max;
	rotate( 'x', init );
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y_range.min = min;
	m_joint_y_range.init = init;
	m_joint_y_range.max = max;
	rotate( 'y', init );
}
