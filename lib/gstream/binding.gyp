{
  "targets": [
    {
      "target_name": "gstream",
      "sources": [ "gstream.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
