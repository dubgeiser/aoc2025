map <Leader>t :wall!\|:let target=expand('%:t:r')\|:execute ":!./build.sh " . target . " && ./build/" . target . " < data/" . target . ".dat"<cr>
