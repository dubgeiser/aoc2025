map <Leader>t :wall!\|:let target=expand('%:t:r')\|:execute ":!./run.sh " . target<cr>
