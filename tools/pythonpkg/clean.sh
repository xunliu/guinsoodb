rm -rf .eggs .pytest_cache build dist guinsoodb.egg-info guinsoodb.cpp guinsoodb.hpp parquet-extension.cpp parquet-extension.hpp guinsoodb guinsoodb_tarball
rm -f sources.list includes.list githash.list
python3 -c "exec('try:\n\timport guinsoodb, shutil, os\n\tshutil.rmtree(guinsoodb.__file__.rsplit(os.path.sep, 1)[0])\nexcept:\n\texit(0);')"