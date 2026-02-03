from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'simulation',
        ['Simulation.cpp', 'Main.cpp'], #Compile files
        include_dirs=[pybind11.get_include()],
        language='c++'
    ),
]

setup(name='simulation', ext_modules=ext_modules)


#python setup.py build_ext --inplace
