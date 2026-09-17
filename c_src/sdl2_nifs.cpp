/*
    This NIF module provides functions to create windows and upate their pixel content using SDL2.

    Made by: Henrique Gabriel Rodrigues
    Oriented and supervised by: Prof. Dr. André Rauber Du Bois
*/

#include <erl_nif.h>
#include <cstdlib>

#include "SDL2Interface/SDL2Interface.hpp"

// Global SDL2 interface instance
SDL2Interface *sdl2 = nullptr;

// This function is called when the NIF library is loaded. It initializes the SDL2 interface.
static int init_nifs(ErlNifEnv *env, void ** /* priv_data */, ERL_NIF_TERM /* load_info */)
{
  if (sdl2 != nullptr)
    return 0; // Already initialized

  try
  {
    sdl2 = new SDL2Interface();
  }
  catch (const std::exception &e)
  {
    std::cerr << "[C++ DEMO NIFS] Failed to initialize SDL2 NIFs: " << e.what() << std::endl;
    enif_raise_exception(env, enif_make_string(env, e.what(), ERL_NIF_LATIN1));

    if (sdl2 != nullptr)
    {
      delete sdl2;
      sdl2 = nullptr;
    }

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// This function is called when the NIF library is unloaded. It cleans up the SDL2 interface instance.
static void unload_nifs(ErlNifEnv * /* env */, void * /* priv_data */)
{
  if (sdl2 != nullptr)
  {
    delete sdl2;
    sdl2 = nullptr;
  }
}

// This nif creates a window with the specified title, width, and height using the SDL2 interface.
// Parameters:
// 1 - Window title (as a charlist)
// 2 - Width (int)
// 3 - Height (int)
static ERL_NIF_TERM create_window_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  if (argc != 3)
  {
    std::cerr << "[ERROR] Invalid number of arguments for create_window_nif." << std::endl;
    return enif_make_badarg(env);
  }

  // GET WINDOW NAME
  ERL_NIF_TERM e_window_name = argv[0];
  unsigned int size;
  if (!enif_get_list_length(env, e_window_name, &size))
  {
    std::cerr << "[ERROR] Failed to get window name length." << std::endl;
    return enif_make_badarg(env);
  }

  char window_name[1024];
  enif_get_string(env, e_window_name, window_name, size + 1, ERL_NIF_LATIN1);

  // GET WIDTH AND HEIGHT
  int width, height;
  if (!enif_get_int(env, argv[1], &width) || !enif_get_int(env, argv[2], &height))
  {
    return enif_make_badarg(env);
  }

  // Creating window
  sdl2->createWindow(window_name, width, height);
  
  std::cout << "[C++ SDL2_NIFS] Created window '" << window_name << "' with dimensions " << width << "x" << height << "." << std::endl;
  std::cout << "[C++ SDL2_NIFS] A new thread in C++ was created to run the SDL2 main loop, which will handle rendering and events for this window." << std::endl;

  return enif_make_int(env, 0);
}

// Checks if the window will close
// No parameters taken
static ERL_NIF_TERM will_window_close_nif(ErlNifEnv *env, int /* argc */, const ERL_NIF_TERM /* argv */[])
{
  bool quit = sdl2->willWindowClose();
  return enif_make_atom(env, quit ? "true" : "false");
}

// This NIF updates the texture of the SDL2 window with the new pixel data provided by the Nx array
// Parameters:
// 1 - Array with pixel data (as an Erlang binary)
static ERL_NIF_TERM update_image_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  if (argc != 1)
  {
    std::cerr << "[ERROR] Invalid number of arguments for update_image_nif." << std::endl;
    return enif_make_badarg(env);
  }

  // Get the array
  ErlNifBinary new_pixel_data;
  if (!enif_inspect_binary(env, argv[0], &new_pixel_data))
  {
    std::cerr << "[ERROR] Failed to get array in update_image_nif." << std::endl;
    return enif_make_badarg(env);
  }

  // Check if the binary size matches the expected size for the window dimensions
  if (new_pixel_data.size != sdl2->getTextureSizeBytes())
  {
    std::cerr << "[ERROR] Pixel data size does not match expected size in update_image_nif." << std::endl;
    std::cerr << "Expected size: " << sdl2->getTextureSizeBytes() << " bytes" << std::endl;
    std::cerr << "Got: " << new_pixel_data.size << " bytes" << std::endl;

    return enif_make_badarg(env);
  }

  // Update the SDL2 texture with the new pixel data
  sdl2->updateTexture((int32_t *)new_pixel_data.data);

  return enif_make_int(env, 0);
}

static ErlNifFunc nif_funcs[] = {
    {.name = "create_window_nif", .arity = 3, .fptr = create_window_nif, .flags = 0},
    {.name = "will_window_close_nif", .arity = 0, .fptr = will_window_close_nif, .flags = 0},
    {.name = "update_image_nif", .arity = 1, .fptr = update_image_nif, .flags = 0}};

ERL_NIF_INIT(Elixir.SimpleSDL2, nif_funcs, &init_nifs, NULL, NULL, &unload_nifs)