# This test will generate a 500x500 blue window

defmodule SDL2 do
  def start() do
    SimpleSDL2.create_window("Blue window", 500, 500)

    sdl_pid = spawn(&sdl_loop/0)
    Process.register(sdl_pid, :sdl2)
  end

  defp sdl_loop() do
    receive do
      {:render, img_nx} ->
        SimpleSDL2.update_image(img_nx)

      _ ->
        IO.puts("SDL2 received unknown message")
    end

    case SimpleSDL2.will_window_close?() do
      true -> exit(:window_closed)
      false -> sdl_loop()
    end
  end
end

defmodule Renderer do
  def render() do
    nx = Nx.tensor(255, type: :s32) |> Nx.broadcast({500, 500})
    Process.link(Process.whereis(:sdl2))

    render_loop(nx)
  end

  defp render_loop(nx) do
    start_time = System.monotonic_time()

    send(:sdl2, {:render, nx})

    end_time = System.monotonic_time()
    duration_ms = System.convert_time_unit(end_time - start_time, :native, :millisecond)

    sleep_ms = 16.67 - duration_ms

    if sleep_ms > 0 do
      Process.sleep(round(sleep_ms))
    end

    render_loop(nx)
  end
end

SDL2.start()
Renderer.render()
