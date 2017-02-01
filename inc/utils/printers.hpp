#pragma once

std::ostream& operator<<(std::ostream& os, glm::vec2 v) {
  os << '[' << v.x << ", " << v.y << ']';
  return os;
}

std::ostream& operator<<(std::ostream& os, glm::vec3 v) {
  os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
  return os;
}
