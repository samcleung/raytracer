#include "SceneNode.hpp"

#include "cs488-framework/MathUtils.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

#include "Animator.hpp"

using namespace glm;

// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(mat4()),
	invtrans(mat4()),
	m_animator(nullptr),
	m_nodeId(nodeInstanceCount++)
{

}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
	delete m_animator;
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
	children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
			break;
		case 'z':
			rot_axis = vec3(0,0,1);
			break;
		default:
			break;
	}
	mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( glm::scale(amount) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( glm::translate(amount) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------

bool SceneNode::hit( Ray& ray, HitRecord& rec, float t0, float t1 )
{
	bool is_hit = false;
	HitRecord new_record;
	// Re-express primary ray in local SMCS to perform object intersection
	Ray model_ray = Ray( glm::vec3( invtrans * glm::vec4( ray.origin, 1.0 ) ),
						glm::vec3( invtrans * glm::vec4( ray.direction, 0.0 ) ) );
	
	for ( SceneNode* node : children ) {
		if ( node->hit( model_ray, new_record, t0, t1 ) ) {
			is_hit = true;
			t1 = new_record.t;
			rec = new_record;
		}
	}

	if ( is_hit )
	{
		// Transform location, normal vector back to WCS
		rec.point = glm::vec3( trans * glm::vec4( rec.point, 1.0f ) );
		glm::mat3 mat_transpose( glm::transpose( invtrans ) );
		rec.normal = mat_transpose * rec.normal;
		rec.u = mat_transpose * rec.u;
		rec.v = mat_transpose * rec.v;
	}

	return is_hit;
}

//---------------------------------------------------------------------------------------
void SceneNode::set_animator(Animator* new_anim)
{
	m_animator = new_anim;
}

//---------------------------------------------------------------------------------------
void SceneNode::update(double delta_time)
{
	if (m_animator) m_animator->update(delta_time);
	for ( auto child : children ) { child->update(delta_time); }
}

//---------------------------------------------------------------------------------------
std::ostream& operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}
