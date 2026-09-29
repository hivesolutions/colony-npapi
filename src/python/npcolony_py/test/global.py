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
        self.binie = self._binie([self._text("npcolony")])
        self.binie_b64 = base64.b64encode(self.binie).decode("utf-8")

    def tearDown(self):
        shutil.rmtree(self.target_dir, ignore_errors=True)

    def _binie(self, elements, title=b"npcolony", width=0, height=0):
        data = struct.pack("<256sIII", title, width, height, len(elements))
        for element_type, element in elements:
            data += struct.pack("<II", element_type, len(element)) + element
        return data

    def _text(self, text, text_weight=0, text_italic=0):
        text_encoded = text.encode("utf-8")
        element = struct.pack(
            "<ii256sIIIIIIIIIII",
            0,
            0,
            b"Calibri",
            9,
            1,
            text_weight,
            text_italic,
            0,
            0,
            0,
            0,
            0,
            0,
            len(text_encoded) + 1,
        )
        return (1, element + text_encoded + b"\0")

    def _image(self):
        bitmap = self._bitmap()
        element = struct.pack("<iiIIIIII", 0, 0, 1, 0, 0, 0, 0, len(bitmap))
        return (2, element + bitmap)

    def _bitmap(self):
        bitmap = struct.pack("<2sIHHI", b"BM", 58, 0, 0, 54)
        bitmap += struct.pack("<IiiHHIIiiII", 40, 1, 1, 1, 24, 0, 4, 2835, 2835, 0, 0)
        return bitmap + b"\x00\x00\xff\x00"

    def test_basic(self):
        self.assertEqual(type(npcolony.VERSION), str)
        self.assertEqual(npcolony.VERSION, "1.3.0")

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

    def test_print_base64_invalid(self):
        self.assertRaises(ValueError, lambda: npcolony.print_base64(""))
        self.assertRaises(ValueError, lambda: npcolony.print_base64("QUJDQ"))
        self.assertRaises(ValueError, lambda: npcolony.print_base64("===="))
        self.assertRaises(ValueError, lambda: npcolony.print_base64("!!!!"))
        self.assertRaises(ValueError, lambda: npcolony.print_base64("AA=A"))

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
    def test_print_printer_base64_output_path_padding(self):
        path = os.path.join(self.target_dir, "output.pdf")
        for data in (b"A", b"AB", b"ABC", b"ABCD", b"\xfb\xff"):
            result = npcolony.print_printer_base64(
                "npcolony-test-printer",
                base64.b64encode(data).decode("utf-8"),
                options=dict(output_path=path),
            )
            self.assertEqual(result, 0)
            with open(path, "rb") as file:
                self.assertEqual(file.read(), data)

    @unittest.skipIf(os.name == "nt", "print to file writes the document on unix")
    def test_print_printer_base64_output_path_unicode(self):
        path = os.path.join(self.target_dir, b"sa\xc3\xadda.pdf".decode("utf-8"))
        result = npcolony.print_printer_base64(
            "npcolony-test-printer", self.data_b64, options=dict(output_path=path)
        )
        self.assertEqual(result, 0)
        with open(path, "rb") as file:
            self.assertEqual(file.read(), self.data)

    def test_print_printer_base64_output_path_none(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.binie_b64, options=dict(output_path=None)
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

    def test_print_printer_base64_unknown(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", self.binie_b64
            ),
        )

    def test_print_printer_base64_unknown_unicode(self):
        printer = b"npcolony-impressora-inv\xc3\xa1lida"
        if sys.version_info[0] >= 3:
            printer = printer.decode("utf-8")
        self.assertRaises(
            IOError, lambda: npcolony.print_printer_base64(printer, self.binie_b64)
        )

    def test_print_printer_base64_long_printer(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64("npcolony" * 256, self.binie_b64),
        )

    def test_print_printer_base64_invalid_options(self):
        self.assertRaises(
            IOError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer",
                self.binie_b64,
                options=dict(title=None, media=3.5, scaling=b"none"),
            ),
        )

    def test_print_printer_base64_invalid_data(self):
        self.assertRaises(
            ValueError,
            lambda: npcolony.print_printer_base64("npcolony-test-printer", ""),
        )
        self.assertRaises(
            ValueError,
            lambda: npcolony.print_printer_base64("npcolony-test-printer", "QUJDQ"),
        )
        self.assertRaises(
            ValueError,
            lambda: npcolony.print_printer_base64(
                "npcolony-test-printer", "====", options=dict(title="npcolony")
            ),
        )
        self.assertRaises(
            ValueError,
            lambda: npcolony.print_printer_base64("npcolony-test-printer", "QQ==QUJD"),
        )

    @unittest.skipIf(os.name != "nt", "binie documents are only printed on windows")
    def test_print_printer_base64_invalid_binie(self):
        names = [device["name"] for device in npcolony.get_devices()]
        if not "Microsoft Print to PDF" in names:
            self.skipTest("requires the microsoft print to pdf printer")

        text_type, text = self._text("npcolony")
        image_type, image = self._image()
        options = dict(output_path=os.path.join(self.target_dir, "output.pdf"))
        for data in (
            self.data,
            self.binie[:-1],
            self._binie([(text_type, text[:-1] + b"x")]),
            self._binie([(text_type, text[:100])]),
            self._binie([(image_type, image[:-1])]),
            self._binie([(image_type, image[:20])]),
            self._binie([(0x10000 + text_type, b"")]),
        ):
            self.assertRaises(
                IOError,
                lambda: npcolony.print_printer_base64(
                    "Microsoft Print to PDF",
                    base64.b64encode(data).decode("utf-8"),
                    options=options,
                ),
            )

        data = self._binie([(text_type, text), (image_type, image), (3, b"")])
        result = npcolony.print_printer_base64(
            "Microsoft Print to PDF",
            base64.b64encode(data).decode("utf-8"),
            options=options,
        )
        self.assertEqual(result, 0)

    @unittest.skipIf(os.name != "nt", "binie documents are only printed on windows")
    def test_print_printer_base64_default(self):
        names = [
            device["name"] for device in npcolony.get_devices() if device["is_default"]
        ]
        if names and not names[0] in (
            "Microsoft Print to PDF",
            "Microsoft XPS Document Writer",
        ):
            self.skipTest("requires a file printer as the default printer")

        options = dict(output_path=os.path.join(self.target_dir, "output.pdf"))
        for printer in ("default", ""):
            if names:
                result = npcolony.print_printer_base64(
                    printer, self.binie_b64, options=options
                )
                self.assertEqual(result, 0)
            else:
                self.assertRaises(
                    IOError,
                    lambda: npcolony.print_printer_base64(
                        printer, self.binie_b64, options=options
                    ),
                )

    @unittest.skipIf(os.name != "nt", "gdi objects are only used on windows")
    def test_print_printer_base64_gdi_objects(self):
        names = [device["name"] for device in npcolony.get_devices()]
        if not "Microsoft Print to PDF" in names:
            self.skipTest("requires the microsoft print to pdf printer")

        image_path = os.path.join(self.target_dir, "image.bmp")
        with open(image_path, "wb") as file:
            file.write(self._bitmap())
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
            self._text("npcolony", text_weight, text_italic)
            for text_weight in (0, 1)
            for text_italic in (0, 1)
        ]
        images = [self._image()] * 3
        growths = [
            growth(print_npcolony, self._binie([])),
            growth(print_npcolony, self._binie(texts[:1])),
            growth(print_npcolony, self._binie(texts)),
            growth(print_npcolony, self._binie(images[:1])),
            growth(print_npcolony, self._binie(images)),
        ]
        empty = growth(print_gdi, 0)
        self.assertEqual(
            growths,
            [empty, empty, empty, growth(print_gdi, 1), growth(print_gdi, 3)],
        )
