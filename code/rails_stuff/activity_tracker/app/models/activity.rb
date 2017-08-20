class Activity
  class << self
    def online_key(now)
      "online:#{now.strftime('%M')}"
    end

    def online_keys(now)
      (0..track_for).collect { |t| online_key(now - t.minutes) }
    end

    def activity_key(object)
      "#{object.class.name.downcase}:#{object.id}"
    end

    def track_for
      15
    end

    def expire
      track_for.minutes
    end

    def track_object(object)
      now = Time.now
      $redis.pipelined do
        $redis.sadd(online_key(now), object.id)
        $redis.expire(online_key(now), expire)
        $redis.set(activity_key(object), now.to_i)
      end
    end

    def online
      $redis.sunion online_keys(Time.now)
    end

    def online_count
      online.count
    end
  end
end
