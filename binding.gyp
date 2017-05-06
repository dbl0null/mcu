{
  "targets": [
    {
      "target_name": "mcu",
      "sources": [ "src/mcu.cc", "src/conference.cc" ],
	  "conditions" : [
		["OS=='linux'", {
			"include_dirs": [
				'<!@(pkg-config gstreamer-1.0 --cflags-only-I | sed s/-I//g)',
				'<!@(pkg-config gstreamer-app-1.0 --cflags-only-I | sed s/-I//g)',
				'<!@(pkg-config gstreamer-app-1.0 --cflags-only-I | sed s/-I//g)',
                '<!@(pkg-config nice --cflags-only-I | sed s/-I//g)'
                '<!@(pkg-config glib-2.0 --cflags-only-I | sed s/-I//g)'
			],
			"libraries": [
				'<!@(pkg-config gstreamer-1.0 --libs)',
				'<!@(pkg-config gstreamer-app-1.0 --libs)',
				'<!@(pkg-config gstreamer-video-1.0 --libs)',
                '<!@(pkg-config nice --libs)',
                '<!@(pkg-config glib-2.0 --libs)'
			]
		}],
		["OS=='mac'", {
			"include_dirs": [
				'<!@(pkg-config gstreamer-1.0 --cflags-only-I | sed s/-I//g)',
				'<!@(pkg-config gstreamer-app-1.0 --cflags-only-I | sed s/-I//g)',
				'<!@(pkg-config gstreamer-app-1.0 --cflags-only-I | sed s/-I//g)',
                '<!@(pkg-config nice --cflags-only-I | sed s/-I//g)',
                '<!@(pkg-config glib-2.0 --cflags-only-I | sed s/-I//g)'
			],
			"libraries": [
				'<!@(pkg-config gstreamer-1.0 --libs)',
				'<!@(pkg-config gstreamer-app-1.0 --libs)',
				'<!@(pkg-config gstreamer-video-1.0 --libs)',
                '<!@(pkg-config nice --libs)',
                '<!@(pkg-config glib-2.0 --libs)'
			]
		}],
		["OS=='win'", {
			"include_dirs": [
				"X:/gstreamer-sdk/1.0/x86_64/include/gstreamer-1.0",
				"X:/gstreamer-sdk/1.0/x86_64/include/glib-2.0",
				"X:/gstreamer-sdk/1.0/x86_64/include/libxml2",
                "X:/gstreamer-sdk/1.0/x86_64/include/nice",
                "X:/gstreamer-sdk/1.0/x86_64/include/glib-2.0"
			],
			"libraries": [
				"X:/gstreamer-sdk/1.0/x86_64/lib/gstreamer-1.0.lib",
				"X:/gstreamer-sdk/1.0/x86_64/lib/gstapp-1.0.lib",
				"X:/gstreamer-sdk/1.0/x86_64/lib/gstvideo-1.0.lib",
				"X:/gstreamer-sdk/1.0/x86_64/lib/gobject-2.0.lib",
                "X:/gstreamer-sdk/1.0/x86_64/lib/nice.lib",
				"X:/gstreamer-sdk/1.0/x86_64/lib/glib-2.0.lib",
			]
		}]
	  ]
    }
  ]
}
