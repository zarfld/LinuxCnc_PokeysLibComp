from setuptools import setup, find_packages

setup(
    name='pokeys_py',
    version='0.1.0',
    packages=find_packages(),
    install_requires=[
        'ctypes',
    ],
    entry_points={
        'console_scripts': [
            'pokeys_py=pokeys_py.__main__:main',
        ],
    },
    author='Dominik Zarfl',
    author_email='dominik.zarfl@example.com',
    description='PoKeys Python library for interfacing with PoKeys devices',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/zarfld/LinuxCnc_PokeysLibComp',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],
    keywords='pokeys, linuxcnc, python, library, real-time',
    project_urls={
        'Bug Reports': 'https://github.com/zarfld/LinuxCnc_PokeysLibComp/issues',
        'Source': 'https://github.com/zarfld/LinuxCnc_PokeysLibComp',
    },
)
