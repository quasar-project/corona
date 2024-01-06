import subprocess


def run_conan(build_type):
    subprocess.run(
        (
            'conan', 'install', '.',
            # '--profile', 'conan/profiles/mingw_windows_x86_64',
            '--profile', 'conan/profiles/msvc_windows_x86_64',
            '--build', 'missing',
            '--output-folder=./dependencies',
            f'--settings=build_type={build_type}'
        )
    )


# def run_cmake():
#     subprocess.run(
#         (
#             'cmake', '..',
#         )
#     )


if __name__ == '__main__':
    run_conan('Debug')
    run_conan('Release')


