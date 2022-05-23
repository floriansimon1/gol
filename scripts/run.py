#!/usr/bin/env python3

import os
import subprocess

import scripts_paths

subprocess.check_call([os.path.join(scripts_paths.output_directory_path, "source", "Debug", "gol")])
