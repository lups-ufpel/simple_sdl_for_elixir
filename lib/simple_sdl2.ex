defmodule SimpleSDL2 do
  @on_load :load_nifs
  defp load_nifs do
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

  # --------------- Public functions ---------------

  def create_window(title, width, height), do: create_window_nif(to_charlist(title), width, height)

  def will_window_close?(), do: will_window_close_nif()

  def update_image(%Nx.Tensor{data: data}) do
    %Nx.BinaryBackend{state: bin_array} = data

    update_image_nif(bin_array)
  end

  # --------------- NIF stubs ---------------
  defp create_window_nif(_title, _width, _height) do
    :erlang.nif_error(:nif_not_implemented)
  end

  defp will_window_close_nif() do
    :erlang.nif_error(:nif_not_implemented)
  end

  defp update_image_nif(_bin_array) do
    :erlang.nif_error(:nif_not_implemented)
  end
end
