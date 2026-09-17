defmodule SimpleSDL2.MixProject do
  use Mix.Project

  def project do
    [
      app: :simple_sdl2,
      version: "0.1.0",
      elixir: "~> 1.17",
      start_permanent: Mix.env() == :prod,
      deps: deps(),

      compilers: [:cmake_compiler] ++ Mix.compilers(),
      cmake_build_dir: "CMakeBuild",
      cmake_source_dirs: ["c_src", "CMakeLists.txt"],
      cmake_targets: ["priv/sdl2_nifs"]
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:cmake_compiler, git: "https://github.com/lups-ufpel/poly_hok.git", sparse: "cmake_compiler"}
    ]
  end
end
