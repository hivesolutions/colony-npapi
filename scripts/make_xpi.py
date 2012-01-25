import os
import sys
import zipfile

target_path = sys.argv[1]
base_path = sys.argv[2]

directory_path = os.path.dirname(os.path.realpath(__file__))
specification_path = os.path.normpath(os.path.join(directory_path, "../src/colony_npapi/descriptors/install.rdf"))
library_path = os.path.normpath(os.path.join(directory_path, "../" + base_path + "/npcolony.dll"))

zip_file = zipfile.ZipFile(target_path + "/npcolony.xpi", "w")
try:
    zip_file.write(specification_path, "install.rdf")
    zip_file.write(library_path, "plugins/npcolony.dll")
finally:
    zip_file.close()
