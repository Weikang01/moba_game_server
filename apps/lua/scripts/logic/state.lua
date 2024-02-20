local State = {
    inView   = 1, -- players can leave
    Ready    = 2, -- all players prepared
    Start    = 3, -- game starts
    Playing  = 4, -- in gameplay
    Checkout = 5, -- game finished
}

return State
