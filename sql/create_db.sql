BEGIN;


CREATE TABLE IF NOT EXISTS public.sensors
(
    id integer NOT NULL,
    name "char" NOT NULL,
	description "char",
    PRIMARY KEY (id)
);

ALTER TABLE public.sensors OWNER TO pi

CREATE TABLE IF NOT EXISTS public.soil_measurements
(
    id SERIAL PRIMARY KEY,
    sensor_id integer NOT NULL,
    moisture integer,
    temp numeric,
	measured_at timestamp DEFAULT now()
);

ALTER TABLE public.soil_measurements OWNER TO pi;
END;