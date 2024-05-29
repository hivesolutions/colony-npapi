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

specification_path = os.path.normpath(
    os.path.join(directory_path, "../src/colony_npapi/descriptors/manifest.json")
)
library_win_path = os.path.normpath(
    os.path.join(directory_path, "../" + base_path + "/npcolony.dll")
)
library_linux_path = os.path.normpath(
    os.path.join(directory_path, "../" + base_path + "/libnpcolony.so")
)
library_mac_path = os.path.normpath(
    os.path.join(directory_path, "../" + base_path + "/npcolony.plugin")
)
images_path = os.path.normpath(
    os.path.join(directory_path, "../src/colony_npapi/descriptors/images")
)

shutil.copyfile(specification_path, temporary_path + "/manifest.json")
shutil.copyfile(library_win_path, temporary_path + "/npcolony.dll")
shutil.copytree(images_path, temporary_path + "/images")
if os.path.exists(library_linux_path):
    shutil.copyfile(library_linux_path, temporary_path + "/libnpcolony.so")
if os.path.exists(library_mac_path):
    shutil.copytree(library_mac_path, temporary_path + "/npcolony.plugin")

try:
    if os.name in ("nt", "os"):
        local_app_path = os.environ["LOCALAPPDATA"]
        program_files_path = os.environ["PROGRAMFILES"]
        program_files_path = os.environ.get("PROGRAMFILES(X86)", program_files_path)
        chrome_path = "%s/Google/Chrome/Application/chrome.exe" % local_app_path
        global_chrome_path = (
            "%s/Google/Chrome/Application/chrome.exe" % program_files_path
        )
        if os.path.exists(global_chrome_path):
            chrome_path = global_chrome_path
    else:
        chrome_path = "chrome"

    subprocess.call(
        [chrome_path, "--pack-extension=%s" % temporary_path, "--no-message-box"]
    )
    try:
        shutil.copyfile(directory_path + "/npcolony.crx", target_path + "/npcolony.crx")
    finally:
        os.remove(directory_path + "/npcolony.crx")
        os.remove(directory_path + "/npcolony.pem")
finally:
    shutil.rmtree(temporary_path)
