{
  'targets': [
    {
      'target_name': 'watchdoger',
      'include_dirs' : [
          "<!(node -e \"require('nan')\")"
      ],
      'sources': [ 
      	'src/watchdoger.cpp',
        'src/watchdoger_common.cpp'
      ],
      'conditions': [
      	['OS=="win"', {
      	  'sources+': [
      	    'src/watchdoger_win.cpp'
      	  ]
      	}],
      	['OS=="mac"', {
      	  'sources+': [
      	    'src/watchdoger_mac.cpp'
      	  ]
      	}],
        ['OS=="linux"', {
          'sources+': [
            'src/watchdoger_linux.cpp'
          ]
        }]
      ]
    }
  ]
}