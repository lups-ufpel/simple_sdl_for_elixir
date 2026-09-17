#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <atomic>

class SDL2Interface
{
private:
  // Well, I just discovered -- IN THE FUCKING WORST WAY POSSIBLE -- that SDL2 is NOT thread safe.
  // So, I'll have to make sure that all SDL2 calls are made from the same thread. The Erlang BEAM VM can
  // schedule different threads for NIF calls and this would break the GUI and make the system unstable.

  // SDL2 thread
  std::thread sdlThread;
  
  // Pixel buffer (shared between threads, protected by mutex)
  std::vector<int32_t> pixelBuffer;
  std::mutex bufferMutex;

  // Atomic control flags
  std::atomic<bool> quit;
  std::atomic<bool> hasNewPixels;

  // Window properties
  int windowWidth = 0;
  int windowHeight = 0;
  std::string windowTitle;

  // SDL2 objects (ONLY ACCESSED FROM THE SDL2 THREAD)
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;

  // This is the main loop that runs in the SDL2 thread, responsible for handling events and rendering.
  void sdlMainLoop();

public:
  SDL2Interface();
  ~SDL2Interface();

  void createWindow(const char *title, int width, int height);

  // I'm using int32_t for the pixels because the texture format is RGB888,
  // which is a 24-bit format (8 bits for each color channel, with the alpha channel ignored).
  // This is perfect because the Nx library in Elixir uses signed 32-bit integers to represent pixels,
  // and the most significant byte (MSB) can be ignored, since the RGB888 format does not use an alpha channel.
  // And now this FUCK is thread-safe, so the pixel buffer is protected by a mutex and the control flags are atomic.
  void updateTexture(int32_t *newPixels);

  // Thread-safe check to see if the window has been requested to close
  bool isCloseRequested() const { return quit.load(); }

  size_t getTextureSizeBytes() const { return sizeof(int32_t) * windowWidth * windowHeight; }
  int getWindowWidth() const { return windowWidth; }
  int getWindowHeight() const { return windowHeight; }
};
