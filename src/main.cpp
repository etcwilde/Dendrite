#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>
#include <memory>
#include <thread>

#include <Image.hpp>

#include <unistd.h>

#define WIDTH   500
#define HEIGHT  500

#define PARTICLES 15000

#define SEED  10

#define INITIAL 4

void progress(unsigned int p, unsigned int total) {
  std::cerr << "\r[" << p << "%]";
}

std::ostream& operator<<(std::ostream& os, std::pair<unsigned int, unsigned int> p) {
  os << '<' << p.first << ", " << p.second << '>';
  return os;
}

typedef struct particle {
  std::pair<unsigned int, unsigned int> pos;
} particle;

bool collide(std::set<std::pair<unsigned int, unsigned int>> & dendrite, particle p) {
  auto pos = p.pos;

  for (int x = -1; x < 2; x++) {
    for (int y = -1; y < 2; y++) {
      if (dendrite.count(std::make_pair(pos.first + x, pos.second + y)))
        return true;
    }
  }
  return false;
}

void runDendrite(std::set<std::pair<unsigned int, unsigned int>> &dendrite, unsigned int id) {
  std::mt19937 gen(SEED + id * PARTICLES);
  std::uniform_int_distribution<> movement_dis(0, 2);
  std::uniform_int_distribution<> position_dis(0, WIDTH > HEIGHT ? HEIGHT : WIDTH);

  particle part;

  for (; dendrite.size() <= PARTICLES ;) {
    do{
      part.pos = std::make_pair(position_dis(gen), position_dis(gen));
    } while(dendrite.count(part.pos) != 0);


    for(;;) {
      int x = movement_dis(gen) - 1, y = movement_dis(gen) - 1;
      // Fell off the universe
      if ((int(part.pos.first) + x < 0 || int(part.pos.first) + x > WIDTH) ||
          (int(part.pos.second) + y < 0 || int(part.pos.second) + y > WIDTH)) {
        break;
      }

      if (collide(dendrite, part)) {
        dendrite.insert(part.pos);
        break;
      }

      part.pos = std::make_pair(part.pos.first + x, part.pos.second + y);
    }
  }
}


int main(int argc, char *argv[]) {

  std::set<std::pair<unsigned int, unsigned int>> dendrite;
  PNGImage img(WIDTH, HEIGHT);

  std::cout << "Concurrency: "  << std::thread::hardware_concurrency() << std::endl;

  std::mt19937 gen(SEED);
  std::uniform_int_distribution<> position_dis(0, WIDTH > HEIGHT ? HEIGHT : WIDTH);


  dendrite.insert(std::make_pair(WIDTH / 2, HEIGHT / 2));

  for (int i = 0; i < INITIAL; ++i) {
    std::pair<unsigned int, unsigned int> pos;
    do{
      pos = std::make_pair(position_dis(gen), position_dis(gen));
    } while(dendrite.count(pos) != 0);
    dendrite.insert(pos);
  }

  std::vector<std::thread> thread_pool;
  for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
    thread_pool.push_back(std::thread(runDendrite, std::ref(dendrite), i));
  }

  while (dendrite.size() < PARTICLES) {
    progress((dendrite.size() / static_cast<float>(PARTICLES)) * 100, PARTICLES);
    usleep(10);
  }

  for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
    thread_pool[i].join();
  }



  for (unsigned int x = 0; x < WIDTH; ++x) {
    for (unsigned int y = 0; y < HEIGHT; ++y) {
      if (dendrite.count(std::make_pair(x, y)) != 0) img.set(x, y, {0, 0, 0, 255});
    }
  }

  img.write("test.png");
  return 0;
}
