import os
import sys
import shutil
import subprocess

target_path = sys.argv[1]
base_path = sys.argv[2]

directory_path = os.path.dirname(os.path.realpath(__file__))
temporary_path = os.path.join(directory_path, "npcolony")

not os.path.exists(target_path) and os.mkdir(target_path)
not os.path.exists(temporary_path) and os.mkdir(temporary_path)

specification_path = os.path.normpath(os.path.join(directory_path, "../src/colony_npapi/descriptors/manifest.json"))
library_path = os.path.normpath(os.path.join(directory_path, "../" + base_path + "/npcolony.dll"))
images_path = os.path.normpath(os.path.join(directory_path, "../src/colony_npapi/descriptors/images"))

shutil.copyfile(specification_path, temporary_path + "/manifest.json")
shutil.copyfile(library_path, temporary_path + "/npcolony.dll")
shutil.copytree(images_path, temporary_path + "/images")

try:
    if os.name in ("nt", "os"):
        local_app_path = os.environ["LOCALAPPDATA"]
        chrome_path = "%s/Google/Chrome/Application/chrome.exe" % local_app_path
    else:
        chrome_path = "chrome"

    subprocess.call([chrome_path, "--pack-extension=%s" % temporary_path, "--no-message-box"])
    try:
        shutil.copyfile(directory_path + "/npcolony.crx", target_path + "/npcolony.crx")
    finally:
        os.remove(directory_path + "/npcolony.crx")
        os.remove(directory_path + "/npcolony.pem")
finally:
    shutil.rmtree(temporary_path + "/images")
    os.remove(temporary_path + "/manifest.json")
    os.remove(temporary_path + "/npcolony.dll")
    os.rmdir(temporary_path)
