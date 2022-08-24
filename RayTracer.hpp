// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include <thread>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <iomanip>

#include "utils.hpp"
#include "options.hpp"
#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "PhongMaterial.hpp"
#include "Time.hpp"

struct Resources {
	// What to render
	SceneNode* root;

	// Image to write to, set to a given width and height
	Image& image;

	// Viewing parameters
	glm::vec3& eye;
	const glm::vec3& view;
	const glm::vec3& up;
	double fovy;

	// Lighting parameters
	const glm::vec3& ambient;
	const std::list<Light*>& lights;
#ifdef ANTI_ALIASING
	static const unsigned int ss = rtc::supersamples;
#else
	static const unsigned int ss = 1;
#endif
	~Resources() {
		delete root;
		for (auto light : lights) {
			delete light;
		}
	}
};

class Job {
	glm::uvec2 start;
	uint size;
	Resources* resources = nullptr;
public:
	Job() {}
	Job( uint x, uint y, uint size, Resources* resources )
	: start(x, y), size(size), resources(resources) {}

	Job& operator=(Job& other) {
		start = other.start;
		size = other.size;
		resources = other.resources;
		return *this;
	}
	void operator()();
};

class JobQueue {
public:
	std::queue<Job> q;
	std::mutex mx;
	std::condition_variable cv, cv_done;
	int num_in_prog = 0;
	bool done = false;

	void release_worker_threads() {
		std::unique_lock<std::mutex> lock(mx);
		done = true;
		lock.unlock();
		cv.notify_all();
	}

	void push(const Job& job) {
		std::unique_lock<std::mutex> lock(mx);
		q.emplace(job);
		cv.notify_one();
	}

	bool pop(Job& job) {
		std::unique_lock<std::mutex> lock(mx);
		while (q.empty()) {
			if (done) return false; 
			cv.wait(lock);
		}
		job = q.front();
		q.pop();
		num_in_prog += 1;
		return true;
	}

	void wait() {
		std::unique_lock<std::mutex> lock(mx);
		while (num_in_prog || !q.empty()) {
			cv_done.wait(lock);
		}
	}

	void notifyJobDone() {
		std::unique_lock<std::mutex> lock(mx);
		num_in_prog -= 1;
		cv_done.notify_one();
	}
};

class WorkerThread {
public:
	JobQueue& jobs;
	WorkerThread(JobQueue& jobs)
	: jobs(jobs) {}
	void operator()() {
		Job job;
		while (jobs.pop(job)) {
			job();
			jobs.notifyJobDone();
		}
	}
};

class RayTracer {
public:
	void disco_update();

	RayTracer(
		// What to render
		SceneNode* root,

		// Image to write to, set to a given width and height
		Image& image,

		// Viewing parameters
		glm::vec3& eye,
		const glm::vec3& view,
		const glm::vec3& up,
		double fovy,

		// Lighting parameters
		const glm::vec3& ambient,
		const std::list<Light*>& lights,

		float end_time = 0.f
	);

	~RayTracer();
	
	void render();

	float m_end_time;
	// Multi-threading
	Resources m_shared;
	JobQueue m_jobs;
	std::vector<std::thread> m_threads;
	uint m_chunk_size;
};
