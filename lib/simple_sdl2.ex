defmodule SimpleSDL2 do
  @on_load :load_nifs
  def load_nifs do
    nif_path = Application.app_dir(:simple_sdl2, "priv/sdl2_nifs") |> to_charlist()
    ret = :erlang.load_nif(nif_path, 0)

    case ret do
      :ok ->
        :ok

      {:error, {reason, text}} ->
        IO.puts("[SimpleSDL2] Failed to load NIF")
        IO.puts("[SimpleSDL2] Reason: #{inspect(reason)}")
        IO.puts("[SimpleSDL2] Text: #{text}")

        :erlang.halt(1)
    end
  end

  def create_window_nif(_title, _width, _height) do
    :erlang.nif_error(:nif_not_implemented)
  end

  def close_requested_nif() do
    :erlang.nif_error(:nif_not_implemented)
  end

  def update_image_nif(_img_array) do
    :erlang.nif_error(:nif_not_implemented)
  end
end
