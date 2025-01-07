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

layout(std430, binding = 3) buffer VertexBuffer {
    float vertices[];
};

layout(std430, binding = 4) buffer ElementBuffer {
    uint triangles[];
};

uniform float deltaTime;
uniform vec3 minAABB;
uniform vec3 maxAABB;

uniform float restitution;

uniform uint triangleCount;

bool intersection(vec3 start, vec3 direction, vec3 v0, vec3 v1, vec3 v2, out float t) {
    const float EPSILON = 1e-6;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    vec3 h = cross(direction, edge2);
    float a = dot(edge1, h);

    if (abs(a) < EPSILON) {
        return false;
    }

    float f = 1.0 / a;

    vec3 s = start - v0;

    float u = f * dot(s, h);
    if (u < -EPSILON || u > 1.0 + EPSILON) {
        return false;
    }

    vec3 q = cross(s, edge1);
    float v = f * dot(direction, q);
    if (v < -EPSILON || u + v > 1.0 + EPSILON) {
        return false;
    }

    t = f * dot(edge2, q);
    if (t < -EPSILON) {
        return false;
    }

    return true;
}

void main() {
    const float EPSILON = 1e-6;
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    vec3 oldPos = p.pos;
    p.velocity += p.force * deltaTime;
    vec3 direction = p.velocity * deltaTime;
    
    if (length(direction) > EPSILON) {
        vec3 newPos = oldPos + direction;

        float t = 2.0;
        vec3 normal;

        for (uint i = 0; i < triangleCount; ++i) {
            float nt;
            uint t1 = triangles[3 * i];
            uint t2 = triangles[3 * i + 1];
            uint t3 = triangles[3 * i + 2];

            vec3 v1 = vec3(vertices[3 * t1], vertices[3 * t1 + 1], vertices[3 * t1 + 2]);
            vec3 v2 = vec3(vertices[3 * t2], vertices[3 * t2 + 1], vertices[3 * t2 + 2]);
            vec3 v3 = vec3(vertices[3 * t3], vertices[3 * t3 + 1], vertices[3 * t3 + 2]);
            vec3 n = cross(v2 - v1, v3 - v1);

            if (dot(direction, n) <= 0.0 && intersection(oldPos, direction, v1, v2, v3, nt) && nt < t) {
                t = nt;
                normal = normalize(n);
            }
        }

        if (t <= 1.0) {
            //newPos = oldPos + t * direction + max(1e-5, 0.01 * length(direction)) * normal;

            newPos = oldPos + t * direction + 1e-5 * normal;
            p.velocity = /*restitution*/ 0.8 * reflect(p.velocity, normal);
        }

        p.pos = newPos;
    }

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