
from setuptools import Extension, setup, find_packages

setup(
    name="class_7zip_arch",
    version='1.0',
    description='Python extension for using 7zip.dll (Example in test/test.py)',
    url='https://github.com/aver007/class_7zip_arch',
    python_requires=">=3.8, <4",

    data_files=[
        ("", ["README.txt"]),
        ("", ["LICENSE"]),
        ("", ["class_7z_arch/7z.dll"]),
        ("libs7z", [
            "class_7z_arch/libs7z/bit7z64.lib",
            "class_7z_arch/libs7z/bit7z64_d.lib"
        ]),
        ("libs7z/include", [
            "class_7z_arch/libs7z/include/bit7z.hpp",
            "class_7z_arch/libs7z/include/bit7zlibrary.hpp",
            "class_7z_arch/libs7z/include/bitarchivecreator.hpp",
            "class_7z_arch/libs7z/include/bitarchivehandler.hpp",
            "class_7z_arch/libs7z/include/bitarchiveinfo.hpp",
            "class_7z_arch/libs7z/include/bitarchiveitem.hpp",
            "class_7z_arch/libs7z/include/bitarchiveopener.hpp",
            "class_7z_arch/libs7z/include/bitcompressionlevel.hpp",
            "class_7z_arch/libs7z/include/bitcompressionmethod.hpp",
            "class_7z_arch/libs7z/include/bitcompressor.hpp",
            "class_7z_arch/libs7z/include/bitexception.hpp",
            "class_7z_arch/libs7z/include/bitextractor.hpp",
            "class_7z_arch/libs7z/include/bitformat.hpp",
            "class_7z_arch/libs7z/include/bitguids.hpp",
            "class_7z_arch/libs7z/include/bitinputarchive.hpp",
            "class_7z_arch/libs7z/include/bitmemcompressor.hpp",
            "class_7z_arch/libs7z/include/bitmemextractor.hpp",
            "class_7z_arch/libs7z/include/bitpropvariant.hpp",
            "class_7z_arch/libs7z/include/bitstreamcompressor.hpp",
            "class_7z_arch/libs7z/include/bitstreamextractor.hpp",
            "class_7z_arch/libs7z/include/bittypes.hpp",
        ]),
        ("", [
            "class_7z_arch/Archive.h",
            "class_7z_arch/class7zarch.h",
            "class_7z_arch/class7zarchiter.h",
            "class_7z_arch/misc.h",
            "class_7z_arch/_main.cpp",
            "class_7z_arch/Archive.cpp",
            "class_7z_arch/class7zarch.cpp",
            "class_7z_arch/class7zarchiter.cpp",
            "class_7z_arch/misc.cpp"
        ]),
        ("test", [
            "test/cats.lzh",
            "test/test.py",
        ]),
    ],

    ext_modules=[
        Extension(
            name = "class_7zip_arch",  # as it would be imported

            sources=[
                "class_7z_arch/_main.cpp",
                "class_7z_arch/Archive.cpp",
                "class_7z_arch/class7zarch.cpp",
                "class_7z_arch/class7zarchiter.cpp",
                "class_7z_arch/misc.cpp"
            ],


            include_dirs = [
                "class_7z_arch/libs7z/include",
            ],
            library_dirs = [
                "class_7z_arch/libs7z",
            ],
            libraries = ["bit7z64",
                         "kernel32",
                         "user32",
                         "gdi32",
                         "winspool",
                         "comdlg32",
                         "advapi32",
                         "shell32",
                         "ole32",
                         "oleaut32",
                         "uuid",
                         "odbc32",
                         "odbccp32",
                         "python38"
            ],

            extra_compile_args =["/MT", "/std:c++17"],
            extra_link_args = [""],
        ),
    ]
)


