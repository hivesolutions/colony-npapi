#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import base64
import ctypes
import shutil
import struct
import tempfile
import unittest

import npcolony


class GlobalTest(unittest.TestCase):

    def setUp(self):
        self.target_dir = tempfile.mkdtemp(prefix="npcolony-test-")
        self.data = b"%PDF-1.4 npcolony test document"
        self.data_b64 = base64.b64encode(self.data).decode("utf-8")

    def tearDown(self):
        shutil.rmtree(self.target_dir, ignore_errors=True)

    def test_basic(self):
        self.assertEqual(type(npcolony.VERSION), str)
        self.assertEqual(npcolony.VERSION, "1.2.10")

        self.assertEqual(type(npcolony.get_devices()), list)

    def test_get_devices(self):
        for device in npcolony.get_devices():
            self.assertEqual(
                sorted(device.keys()),
                [
                    "bottom",
                    "is_default",
                    "left",
                    "length",
                    "media",
                    "name",
                    "right",
                    "top",
                    "width",
                ],
            )
            self.assertEqual(device["left"] <= device["right"] <= device["width"], True)
            self.assertEqual(
                device["bottom"] <= device["top"] <= device["length"], True
            )

    def test_get_devices_references(self):
        devices = npcolony.get_devices()
        controls = [dict(value=float(index) + 0.5) for index in range(len(devices))]
        for index in range(len(devices)):
            self.assertEqual(
                sys.getrefcount(devices[index]), sys.getrefcount(controls[index])
            )
            for key in ("width", "length", "left", "bottom", "right", "top"):
                self.assertEqual(
                    sys.getrefcount(devices[index][key]),
                    sys.getrefcount(controls[index]["value"]),
                )

    @unittest.skipIf(os.name == "nt", "print to file writes the document on unix")
    def test_print_printer_base64_output_path(self):
        path = os.path.join(self.target_dir, "output.pdf")
        result = npcolony.print_printer_base64(
            "npcolony-test-printer", self.data_b64, options=dict(output_path=path)
        )
        self.assertEqual(result, 0)
        with open(path, "rb") as file:
            self.assertEqual(file.read(), self.data)

    @unittest.skipIf(os.name == "nt", "print to file writes the document on unix")
    def test_print_printer_base64_output_path_unicode(self):
        path = os.path.join(self.target_dir, b"sa\xc3\xadda.pdf".decode("utf-8"))
        result = npcolony.print_printer_base64(
            "npcolony-test-printer", self.data_b64, options=dict(output_path=path)
        )
        self.assertEqual(result, 0)
        with open(path, "rb") as file:
            self.assertEqual(file.read(), self.data)

    @unittest.skipIf(os.name == "nt", "printer resolution is only checked on unix")
    def test_print_printer_base64_output_path_none(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.data_b64, options=dict(output_path=None)
            ),
        )

    def test_print_printer_base64_output_path_type(self):
        self.assertRaises(
            TypeError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.data_b64, options=dict(output_path=1)
            ),
        )

    @unittest.skipIf(not os.path.exists("/dev/full"), "requires the full device")
    def test_print_printer_base64_full_disk(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer",
                self.data_b64,
                options=dict(output_path="/dev/full"),
            ),
        )

    @unittest.skipIf(os.name == "nt", "print to file writes the document on unix")
    def test_print_printer_base64_invalid_path(self):
        path = os.path.join(self.target_dir, "missing", "output.pdf")
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.data_b64, options=dict(output_path=path)
            ),
        )

    @unittest.skipIf(os.name == "nt", "printer resolution is only checked on unix")
    def test_print_printer_base64_unknown(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.data_b64
            ),
        )

    @unittest.skipIf(os.name == "nt", "printer resolution is only checked on unix")
    def test_print_printer_base64_invalid_options(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer",
                self.data_b64,
                options=dict(title=None, media=3.5, scaling=b"none"),
            ),
        )

    @unittest.skipIf(os.name != "nt", "gdi objects are only used on windows")
    def test_print_printer_base64_gdi_objects(self):
        names = [device["name"] for device in npcolony.get_devices()]
        if not "Microsoft Print to PDF" in names:
            self.skipTest("requires the microsoft print to pdf printer")

        def element(value, weight=0, italic=0, text=True):
            if text:
                return (
                    struct.pack(
                        "<IIii256s11I",
                        1,
                        308 + len(value),
                        0,
                        0,
                        b"Calibri",
                        9,
                        1,
                        weight,
                        italic,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        len(value),
                    )
                    + value
                )
            return (
                struct.pack(
                    "<IIii6I", 2, 32 + len(value), 0, 0, 1, 0, 0, 0, 0, len(value)
                )
                + value
            )

        def document(*elements):
            header = struct.pack("<256sIII", b"npcolony", 0, 0, len(elements))
            return header + b"".join(elements)

        text = b"npcolony\x00"
        image = struct.pack("<2sIHHI", b"BM", 58, 0, 0, 54)
        image += struct.pack("<IiiHHIIiiII", 40, 1, 1, 1, 24, 0, 4, 2835, 2835, 0, 0)
        image += b"\x00\x00\xff\x00"
        image_path = os.path.join(self.target_dir, "image.bmp")
        with open(image_path, "wb") as file:
            file.write(image)
        path = os.path.join(self.target_dir, "output.pdf")

        class DocInfo(ctypes.Structure):
            _fields_ = [
                ("size", ctypes.c_int),
                ("name", ctypes.c_wchar_p),
                ("output", ctypes.c_wchar_p),
                ("datatype", ctypes.c_wchar_p),
                ("type", ctypes.c_uint),
            ]

        gdi32 = ctypes.WinDLL("gdi32")
        gdi32.CreateDCW.restype = ctypes.c_void_p
        gdi32.CreateDCW.argtypes = (ctypes.c_wchar_p,) * 3 + (ctypes.c_void_p,)
        gdi32.CreateCompatibleDC.restype = ctypes.c_void_p
        gdi32.SelectObject.restype = ctypes.c_void_p
        gdi32.SelectObject.argtypes = (ctypes.c_void_p, ctypes.c_void_p)
        gdi32.StretchBlt.argtypes = (
            (ctypes.c_void_p,)
            + (ctypes.c_int,) * 4
            + (ctypes.c_void_p,)
            + (ctypes.c_int,) * 4
            + (ctypes.c_uint,)
        )
        gdi32.StartDocW.argtypes = (ctypes.c_void_p, ctypes.POINTER(DocInfo))
        for name in (
            "CreateCompatibleDC",
            "DeleteObject",
            "StartPage",
            "EndPage",
            "EndDoc",
            "DeleteDC",
        ):
            getattr(gdi32, name).argtypes = (ctypes.c_void_p,)
        kernel32 = ctypes.WinDLL("kernel32")
        kernel32.GetCurrentProcess.restype = ctypes.c_void_p
        user32 = ctypes.WinDLL("user32")
        user32.GetGuiResources.argtypes = (ctypes.c_void_p, ctypes.c_uint)
        user32.LoadImageW.restype = ctypes.c_void_p
        user32.LoadImageW.argtypes = (
            ctypes.c_void_p,
            ctypes.c_wchar_p,
            ctypes.c_uint,
            ctypes.c_int,
            ctypes.c_int,
            ctypes.c_uint,
        )
        process = kernel32.GetCurrentProcess()

        def print_gdi(count):
            context = gdi32.CreateDCW("WINSPOOL", "Microsoft Print to PDF", None, None)
            info = DocInfo(ctypes.sizeof(DocInfo), "npcolony", path, None, 0)
            gdi32.StartDocW(context, ctypes.byref(info))
            gdi32.StartPage(context)
            for _index in range(count):
                bitmap = user32.LoadImageW(None, image_path, 0, 0, 0, 0x50)
                image_context = gdi32.CreateCompatibleDC(None)
                previous = gdi32.SelectObject(image_context, bitmap)
                result = gdi32.StretchBlt(
                    context, 0, 0, 10, 10, image_context, 0, 0, 1, 1, 0xCC0020
                )
                self.assertEqual(result != 0, True)
                gdi32.SelectObject(image_context, previous)
                self.assertEqual(gdi32.DeleteObject(bitmap) != 0, True)
                self.assertEqual(gdi32.DeleteDC(image_context) != 0, True)
            gdi32.EndPage(context)
            gdi32.EndDoc(context)
            gdi32.DeleteDC(context)

        def print_npcolony(data):
            result = npcolony.print_printer_base64(
                "Microsoft Print to PDF",
                base64.b64encode(data).decode("utf-8"),
                options=dict(output_path=path),
            )
            self.assertEqual(result, 0)

        def growth(method, *args):
            counts = []
            for _index in range(8):
                method(*args)
                counts.append(user32.GetGuiResources(process, 0))
            return [count - counts[4] for count in counts[4:]]

        texts = [
            element(text, weight, italic) for weight in (0, 1) for italic in (0, 1)
        ]
        images = [element(image, text=False)] * 3
        growths = [
            growth(print_npcolony, document()),
            growth(print_npcolony, document(texts[0])),
            growth(print_npcolony, document(*texts)),
            growth(print_npcolony, document(images[0])),
            growth(print_npcolony, document(*images)),
        ]
        empty = growth(print_gdi, 0)
        self.assertEqual(
            growths,
            [empty, empty, empty, growth(print_gdi, 1), growth(print_gdi, 3)],
        )
