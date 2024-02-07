mysql_wrapper.connect("127.0.0.1", 3306, "root", "123", "test_database", function(err, context)
    if (err) then
        print(err)
        return
    end

    mysql_wrapper.close(context);
end);
