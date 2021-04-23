.global branch_to_address
branch_to_address:
    br x0
    ret // We never reach this point

.global awaken
awaken:
    sev

.global get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret