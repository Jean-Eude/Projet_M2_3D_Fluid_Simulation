#version 460 core

layout(local_size_x = 256) in;

// Alignement sur 16 bytes, 32, etc... pour une structure dans les shaders
struct Particule {
    vec3 pos;
    float _pad1; 
    vec3 velocity;
    float _padd2;
    vec3 dir;
    float _pad3;
    vec3 force;
    float _pad4;
    float scale;         
    float life;
    float density;
    int isActive;
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

/*layout(std430, binding = 3) buffer VertexBuffer {
    float vertices[];
};

layout(std430, binding = 4) buffer ElementBuffer {
    uint triangles[];
};*/

uniform float deltaTime;
uniform vec3 minAABB;
uniform vec3 maxAABB;

//uniform uint triangleCount;

uniform float restitution;

bool intersection(vec3 start, vec3 direction, vec3 v0, vec3 v1, vec3 v2, out float t) {
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    vec3 h = cross(direction, edge2);
    float a = dot(edge1, h);

    if (abs(a) < 1e-6) {
        return false;
    }

    float f = 1.0 / a;

    vec3 s = start - v0;

    float u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    vec3 q = cross(s, edge1);
    float v = f * dot(direction, q);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    t = f * dot(edge2, q);
    if (t < 0.0) {
        return false;
    }

    return true;
}

/*void resolveIntersection(vec3 start, vec3 end) {
    uint id = gl_GlobalInvocationID.x;
    vec3 direction = end - start;

    float t = 2.0;
    vec3 newDirection;

    for (uint i = 0; i < triangleCount; ++i) {
        float nt;
        vec3 v1 = vertices[triangles[3 * i]];
        vec3 v2 = vertices[triangles[3 * i + 1]];
        vec3 v3 = vertices[triangles[3 * i + 2]];
        if (intersection(start, direction, v1, v2, v3, nt) && nt < t) {
            t = nt;
            newDirection = reflect(direction, normalize(cross(v3 - v1, v2 - v1)));
        }
    }

    if (t > 0 && t < 1) {
        particles[id].pos = start + t * direction;
        particles[id].velocity = restitution * newDirection;
    }
}*/

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    vec3 oldPos = p.pos;
    p.velocity += p.force * deltaTime;
    p.pos += p.velocity * deltaTime;
    
    for (int i = 0; i < 3; i++) {
        if (p.pos[i] < minAABB[i]) {
            p.pos[i] = 2.0 * minAABB[i] - p.pos[i];
            p.velocity[i] *= -restitution;
        } else if (p.pos[i] > maxAABB[i]) {
            p.pos[i] = 2.0 * maxAABB[i] - p.pos[i];
            p.velocity[i] *= -restitution;
        }
    }

    particles[id] = p;
}