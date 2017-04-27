{
  "targets": [
    {
      "target_name": "gstream",
      "sources": [ "gstream.cc", "stream.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
