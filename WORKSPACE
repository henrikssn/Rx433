load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

new_git_repository(
    name = "googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.7.0",
    build_file = "BUILD.googletest",
)

new_git_repository(
    name = "googlemock",
    remote = "https://github.com/google/googlemock",
    tag = "release-1.7.0",
    build_file = "BUILD.googlemock",
)
new_git_repository(
    name = "arduino-mock",
    remote = "https://github.com/balp/arduino-mock",
    commit = "0d46e11cd65a6cbd3784d63b95dc28bed709dcca",
    build_file = "BUILD.arduino-mock"
)
