{
  "targets": [
    {
      "target_name": "gstream",
      "sources": [ "gstream.cc", "stream.cc", "conference.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
